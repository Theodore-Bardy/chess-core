/**
 * @file  database.cpp
 * @brief SQLite database layer implementation
 */

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "database.hpp"

/* ------------------------------------------------------------------ */
/* Schema                                                               */
/* ------------------------------------------------------------------ */

static const char* SCHEMA_SQL = R"SQL(
CREATE TABLE IF NOT EXISTS players (
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT    NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS games (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    pgn_text        TEXT    NOT NULL,
    result          TEXT    NOT NULL DEFAULT '*',
    date            TEXT    NOT NULL DEFAULT '',
    white_player_id INTEGER REFERENCES players(id),
    black_player_id INTEGER REFERENCES players(id),
    white_elo       INTEGER NOT NULL DEFAULT 0,
    black_elo       INTEGER NOT NULL DEFAULT 0,
    eco             TEXT    NOT NULL DEFAULT '',
    opening         TEXT    NOT NULL DEFAULT '',
    event           TEXT    NOT NULL DEFAULT '',
    site            TEXT    NOT NULL DEFAULT '',
    round           TEXT    NOT NULL DEFAULT ''
);

CREATE TABLE IF NOT EXISTS positions (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    fen        TEXT    NOT NULL,
    game_id    INTEGER NOT NULL REFERENCES games(id) ON DELETE CASCADE,
    move_num   INTEGER NOT NULL,
    eval_score REAL
);

CREATE INDEX IF NOT EXISTS idx_positions_fen     ON positions(fen);
CREATE INDEX IF NOT EXISTS idx_games_white       ON games(white_player_id);
CREATE INDEX IF NOT EXISTS idx_games_black       ON games(black_player_id);
CREATE INDEX IF NOT EXISTS idx_games_result      ON games(result);
CREATE INDEX IF NOT EXISTS idx_games_eco         ON games(eco);
)SQL";

/* ------------------------------------------------------------------ */
/* Construction                                                         */
/* ------------------------------------------------------------------ */

Database::Database(std::string const& path)
    : db(path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    db.exec("PRAGMA foreign_keys = ON;");
    db.exec("PRAGMA journal_mode = WAL;");
    initSchema();
}

void Database::initSchema()
{
    db.exec(SCHEMA_SQL);
}

/* ------------------------------------------------------------------ */
/* Private helpers                                                      */
/* ------------------------------------------------------------------ */

int Database::upsertPlayer(std::string const& name)
{
    if (name.empty())
        return 0;

    /* Try to find existing row */
    {
        SQLite::Statement sel(db, "SELECT id FROM players WHERE name = ?");
        sel.bind(1, name);
        if (sel.executeStep())
            return sel.getColumn(0).getInt();
    }

    /* Insert new row */
    SQLite::Statement ins(db, "INSERT INTO players (name) VALUES (?)");
    ins.bind(1, name);
    ins.exec();
    return static_cast<int>(db.getLastInsertRowid());
}

GameMetadata Database::recordToMeta(GameRecord const& rec)
{
    GameMetadata m;
    m.event    = rec.event;
    m.site     = rec.site;
    m.date     = rec.date;
    m.round    = rec.round;
    m.white    = rec.white;
    m.black    = rec.black;
    m.result   = rec.result;
    m.whiteElo = rec.whiteElo;
    m.blackElo = rec.blackElo;
    m.eco      = rec.eco;
    m.opening  = rec.opening;
    return m;
}

/* ------------------------------------------------------------------ */
/* PGN import                                                           */
/* ------------------------------------------------------------------ */

int Database::importPGN(std::string const& filePath)
{
    /* Read the entire file so we can split on game boundaries */
    std::ifstream file(filePath);
    if (!file.is_open())
        return -1;

    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();

    /* Split file into individual game blocks.
     * A new game begins with the first '[' on a fresh header section.
     * We detect the boundary: a blank line followed by a '[' character. */
    std::vector<std::string> blocks;
    {
        std::istringstream ss(content);
        std::string        line;
        std::string        current;
        bool               prevBlank = false;

        while (std::getline(ss, line))
        {
            bool isBlank = line.find_first_not_of(" \t\r\n") == std::string::npos;

            if (prevBlank && !line.empty() && line[0] == '[' && !current.empty())
            {
                blocks.push_back(current);
                current.clear();
            }
            current += line + '\n';
            prevBlank = isBlank;
        }
        if (!current.empty())
            blocks.push_back(current);
    }

    int imported = 0;

    SQLite::Transaction transaction(db);

    for (std::string const& block : blocks)
    {
        /* Write to a temp file so the Parser (which works on file paths) can read it */
        std::string tmpPath = filePath + ".tmp_import";
        {
            std::ofstream tmp(tmpPath);
            if (!tmp.is_open())
                continue;
            tmp << block;
        }

        GameMetadata meta;
        Game         game;
        Parser       parser;

        if (!parser.loadGame(game, tmpPath, &meta))
        {
            std::remove(tmpPath.c_str());
            continue;
        }
        std::remove(tmpPath.c_str());

        /* Serialize just the move text (no headers) for storage */
        std::string moveTmpPath = filePath + ".tmp_moves";
        parser.saveGame(game, moveTmpPath, nullptr);
        std::string pgnText;
        {
            std::ifstream mf(moveTmpPath);
            if (mf.is_open())
            {
                pgnText.assign((std::istreambuf_iterator<char>(mf)),
                                std::istreambuf_iterator<char>());
                mf.close();
            }
        }
        std::remove(moveTmpPath.c_str());

        int whiteId = upsertPlayer(meta.white);
        int blackId = upsertPlayer(meta.black);

        /* Insert game row */
        SQLite::Statement ins(db,
            "INSERT INTO games "
            "(pgn_text, result, date, white_player_id, black_player_id, "
            " white_elo, black_elo, eco, opening, event, site, round) "
            "VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");
        ins.bind(1,  pgnText);
        ins.bind(2,  meta.result.empty() ? std::string("*") : meta.result);
        ins.bind(3,  meta.date);
        ins.bind(4,  whiteId);
        ins.bind(5,  blackId);
        ins.bind(6,  meta.whiteElo);
        ins.bind(7,  meta.blackElo);
        ins.bind(8,  meta.eco);
        ins.bind(9,  meta.opening);
        ins.bind(10, meta.event);
        ins.bind(11, meta.site);
        ins.bind(12, meta.round);
        ins.exec();

        int gameId = static_cast<int>(db.getLastInsertRowid());

        /* Insert one row per position (FEN) reached during the game */
        {
            SQLite::Statement posIns(db,
                "INSERT INTO positions (fen, game_id, move_num) VALUES (?,?,?)");

            /* Walk the main line of the game tree to collect FEN snapshots */
            GameTree const& tree = game.getTree();
            GameNode const* node = tree.getRoot();
            int             moveNum = 0;

            /* Root node = starting position */
            posIns.bind(1, node->board.getFEN(true, 1));
            posIns.bind(2, gameId);
            posIns.bind(3, moveNum);
            posIns.exec();
            posIns.reset();

            bool isWhite = true;
            int  fullMove = 1;

            while (!node->children.empty())
            {
                node = node->children[0]; /* main line only */
                ++moveNum;
                if (!isWhite) ++fullMove;
                isWhite = !isWhite;

                posIns.bind(1, node->board.getFEN(isWhite, fullMove));
                posIns.bind(2, gameId);
                posIns.bind(3, moveNum);
                posIns.exec();
                posIns.reset();
            }
        }

        ++imported;
    }

    transaction.commit();
    return imported;
}

/* ------------------------------------------------------------------ */
/* Queries                                                              */
/* ------------------------------------------------------------------ */

std::vector<GameRecord> Database::searchGames(GameFilter const& filter) const
{
    std::string sql =
        "SELECT g.id, g.result, g.date, p1.name, p2.name, "
        "       g.white_elo, g.black_elo, g.eco, g.opening, g.event, g.site, g.round "
        "FROM games g "
        "LEFT JOIN players p1 ON g.white_player_id = p1.id "
        "LEFT JOIN players p2 ON g.black_player_id = p2.id "
        "WHERE 1=1 ";

    if (!filter.white.empty())    sql += "AND p1.name LIKE ? ";
    if (!filter.black.empty())    sql += "AND p2.name LIKE ? ";
    if (!filter.result.empty())   sql += "AND g.result = ? ";
    if (!filter.dateFrom.empty()) sql += "AND g.date >= ? ";
    if (!filter.dateTo.empty())   sql += "AND g.date <= ? ";
    if (!filter.eco.empty())      sql += "AND g.eco LIKE ? ";
    if (filter.minElo > 0)        sql += "AND (g.white_elo + g.black_elo) / 2 >= ? ";
    if (filter.maxElo > 0)        sql += "AND (g.white_elo + g.black_elo) / 2 <= ? ";
    sql += "ORDER BY g.id DESC";

    SQLite::Statement q(db, sql);
    int idx = 1;
    if (!filter.white.empty())    q.bind(idx++, "%" + filter.white + "%");
    if (!filter.black.empty())    q.bind(idx++, "%" + filter.black + "%");
    if (!filter.result.empty())   q.bind(idx++, filter.result);
    if (!filter.dateFrom.empty()) q.bind(idx++, filter.dateFrom);
    if (!filter.dateTo.empty())   q.bind(idx++, filter.dateTo);
    if (!filter.eco.empty())      q.bind(idx++, filter.eco + "%");
    if (filter.minElo > 0)        q.bind(idx++, filter.minElo);
    if (filter.maxElo > 0)        q.bind(idx++, filter.maxElo);

    std::vector<GameRecord> results;
    while (q.executeStep())
    {
        GameRecord r;
        r.id       = q.getColumn(0).getInt();
        r.result   = q.getColumn(1).getString();
        r.date     = q.getColumn(2).getString();
        r.white    = q.getColumn(3).isNull() ? "" : q.getColumn(3).getString();
        r.black    = q.getColumn(4).isNull() ? "" : q.getColumn(4).getString();
        r.whiteElo = q.getColumn(5).getInt();
        r.blackElo = q.getColumn(6).getInt();
        r.eco      = q.getColumn(7).getString();
        r.opening  = q.getColumn(8).getString();
        r.event    = q.getColumn(9).getString();
        r.site     = q.getColumn(10).getString();
        r.round    = q.getColumn(11).getString();
        results.push_back(r);
    }
    return results;
}

GameRecord Database::getGame(int id) const
{
    SQLite::Statement q(db,
        "SELECT g.id, g.pgn_text, g.result, g.date, p1.name, p2.name, "
        "       g.white_elo, g.black_elo, g.eco, g.opening, g.event, g.site, g.round "
        "FROM games g "
        "LEFT JOIN players p1 ON g.white_player_id = p1.id "
        "LEFT JOIN players p2 ON g.black_player_id = p2.id "
        "WHERE g.id = ?");
    q.bind(1, id);

    GameRecord r;
    if (q.executeStep())
    {
        r.id       = q.getColumn(0).getInt();
        r.pgnText  = q.getColumn(1).getString();
        r.result   = q.getColumn(2).getString();
        r.date     = q.getColumn(3).getString();
        r.white    = q.getColumn(4).isNull() ? "" : q.getColumn(4).getString();
        r.black    = q.getColumn(5).isNull() ? "" : q.getColumn(5).getString();
        r.whiteElo = q.getColumn(6).getInt();
        r.blackElo = q.getColumn(7).getInt();
        r.eco      = q.getColumn(8).getString();
        r.opening  = q.getColumn(9).getString();
        r.event    = q.getColumn(10).getString();
        r.site     = q.getColumn(11).getString();
        r.round    = q.getColumn(12).getString();
    }
    return r;
}

std::vector<GameRecord> Database::getGamesByPosition(std::string const& fen) const
{
    SQLite::Statement q(db,
        "SELECT g.id, g.result, g.date, p1.name, p2.name, "
        "       g.white_elo, g.black_elo, g.eco, g.opening, g.event, g.site, g.round "
        "FROM positions pos "
        "JOIN games g ON pos.game_id = g.id "
        "LEFT JOIN players p1 ON g.white_player_id = p1.id "
        "LEFT JOIN players p2 ON g.black_player_id = p2.id "
        "WHERE pos.fen = ? "
        "ORDER BY g.id DESC");
    q.bind(1, fen);

    std::vector<GameRecord> results;
    while (q.executeStep())
    {
        GameRecord r;
        r.id       = q.getColumn(0).getInt();
        r.result   = q.getColumn(1).getString();
        r.date     = q.getColumn(2).getString();
        r.white    = q.getColumn(3).isNull() ? "" : q.getColumn(3).getString();
        r.black    = q.getColumn(4).isNull() ? "" : q.getColumn(4).getString();
        r.whiteElo = q.getColumn(5).getInt();
        r.blackElo = q.getColumn(6).getInt();
        r.eco      = q.getColumn(7).getString();
        r.opening  = q.getColumn(8).getString();
        r.event    = q.getColumn(9).getString();
        r.site     = q.getColumn(10).getString();
        r.round    = q.getColumn(11).getString();
        results.push_back(r);
    }
    return results;
}

void Database::saveAnalysis(int gameId, int moveNum, float evalScore)
{
    SQLite::Statement q(db,
        "UPDATE positions SET eval_score = ? WHERE game_id = ? AND move_num = ?");
    q.bind(1, static_cast<double>(evalScore));
    q.bind(2, gameId);
    q.bind(3, moveNum);
    q.exec();
}

PlayerStats Database::getPlayerStats(std::string const& name) const
{
    PlayerStats stats;
    stats.name = name;

    SQLite::Statement q(db,
        "SELECT g.result, "
        "       (g.white_player_id = p.id) AS is_white "
        "FROM games g "
        "JOIN players p ON (g.white_player_id = p.id OR g.black_player_id = p.id) "
        "WHERE p.name = ?");
    q.bind(1, name);

    while (q.executeStep())
    {
        std::string result  = q.getColumn(0).getString();
        bool        isWhite = q.getColumn(1).getInt() != 0;

        ++stats.gamesPlayed;

        if (result == "1-0")
            isWhite ? ++stats.wins : ++stats.losses;
        else if (result == "0-1")
            isWhite ? ++stats.losses : ++stats.wins;
        else if (result == "1/2-1/2")
            ++stats.draws;
    }

    return stats;
}
