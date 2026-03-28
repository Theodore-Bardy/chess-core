/**
 * @file  statistics.cpp
 * @brief Chess statistics and blunder detection implementation
 */

#include "statistics.hpp"

#include <cstdio>
#include <fstream>
#include <map>
#include <sstream>

Statistics::Statistics(Database& _db, UCIEngine& _engine)
    : db(_db)
    , engine(_engine)
{}

/* ------------------------------------------------------------------ */
/* analyzeGame                                                          */
/* ------------------------------------------------------------------ */

int Statistics::analyzeGame(int gameId, int depth)
{
    GameRecord rec = db.getGame(gameId);
    if (rec.id == 0)
        return -1;

    /* Write a minimal PGN to a temp file so Parser can load it */
    std::string tmpPath = "/tmp/chess_analyze_" + std::to_string(gameId) + ".pgn";
    {
        std::ofstream f(tmpPath);
        if (!f.is_open()) return -1;
        f << "[White \"" << rec.white  << "\"]\n";
        f << "[Black \"" << rec.black  << "\"]\n";
        f << "[Result \"" << rec.result << "\"]\n\n";
        f << rec.pgnText << "\n";
    }

    Game   game;
    Parser parser;
    bool   loaded = parser.loadGame(game, tmpPath);
    std::remove(tmpPath.c_str());
    if (!loaded) return -1;

    /* Walk main line collecting FENs and evaluate each position */
    GameTree const& tree     = game.getTree();
    GameNode const* node     = tree.getRoot();
    int             moveNum  = 0;
    bool            isWhite  = true;
    int             fullMove = 1;
    int             evaluated = 0;

    auto evalAndStore = [&](std::string const& fen, int mn)
    {
        engine.setPosition(fen);
        EngineResult res = engine.analyze(depth);
        db.saveAnalysis(gameId, mn, res.score);
        ++evaluated;
    };

    evalAndStore(node->board.getFEN(true, 1), moveNum);

    while (!node->children.empty())
    {
        node = node->children[0];
        ++moveNum;
        if (!isWhite) ++fullMove;
        isWhite = !isWhite;
        evalAndStore(node->board.getFEN(isWhite, fullMove), moveNum);
    }

    return evaluated;
}

/* ------------------------------------------------------------------ */
/* detectBlunders                                                       */
/* ------------------------------------------------------------------ */

std::vector<BlunderInfo> Statistics::detectBlunders(int         gameId,
                                                     float       mistakeThreshold,
                                                     float       blunderThreshold) const
{
    std::vector<std::pair<int, float>> evals = db.getEvals(gameId);
    std::vector<BlunderInfo>           blunders;

    /* We need at least two consecutive evaluations */
    for (size_t i = 1; i < evals.size(); ++i)
    {
        int   mn0       = evals[i - 1].first;
        float evalBefore = evals[i - 1].second;
        float evalAfter  = evals[i].second;

        /* Perspective flip: after White's move, score is from White's POV;
         * after Black's move it is still from White's POV.
         * A blunder is a move that hurts the side to move, i.e. the
         * evaluation shifts in the opponent's favour.
         * Move i-1→i is made by whoever has moveNum mn0 (0=start, 1=White's 1st, ...) */
        bool whiteJustMoved = (evals[i].first % 2 == 1); /* odd moveNum = white moved */
        float drop;
        if (whiteJustMoved)
            drop = evalBefore - evalAfter; /* White wants score high */
        else
            drop = evalAfter - evalBefore; /* Black wants score low */

        if (drop >= mistakeThreshold)
        {
            BlunderInfo b;
            b.moveNum    = evals[i].first;
            b.evalBefore = evalBefore;
            b.evalAfter  = evalAfter;
            b.drop       = drop;
            b.severity   = (drop >= blunderThreshold) ? "blunder" : "mistake";
            blunders.push_back(b);
        }
        (void)mn0;
    }

    return blunders;
}

/* ------------------------------------------------------------------ */
/* getOpeningStats                                                      */
/* ------------------------------------------------------------------ */

OpeningStats Statistics::getOpeningStats(std::string const& eco) const
{
    GameFilter f;
    f.eco = eco;
    std::vector<GameRecord> games = db.searchGames(f);

    OpeningStats stats;
    stats.eco = eco;

    for (GameRecord const& g : games)
    {
        if (stats.name.empty() && !g.opening.empty())
            stats.name = g.opening;

        ++stats.gamesPlayed;

        if      (g.result == "1-0")     ++stats.wins;
        else if (g.result == "0-1")     ++stats.losses;
        else if (g.result == "1/2-1/2") ++stats.draws;
    }

    if (stats.gamesPlayed > 0)
        stats.winRate = static_cast<float>(stats.wins) / stats.gamesPlayed;

    return stats;
}

/* ------------------------------------------------------------------ */
/* getPositionFrequency                                                 */
/* ------------------------------------------------------------------ */

int Statistics::getPositionFrequency(std::string const& fen) const
{
    return static_cast<int>(db.getGamesByPosition(fen).size());
}

/* ------------------------------------------------------------------ */
/* getPerformanceTrend                                                  */
/* ------------------------------------------------------------------ */

std::vector<TrendPoint> Statistics::getPerformanceTrend(std::string const& player,
                                                         std::string const& dateFrom,
                                                         std::string const& dateTo) const
{
    GameFilter f;
    f.dateFrom = dateFrom;
    f.dateTo   = dateTo;

    f.white = player;
    f.black = "";
    std::vector<GameRecord> asWhite = db.searchGames(f);

    f.white = "";
    f.black = player;
    std::vector<GameRecord> asBlack = db.searchGames(f);

    /* Bucket by year-month (first 7 chars of date: "YYYY.MM") */
    std::map<std::string, TrendPoint> buckets;

    auto process_game = [&](GameRecord const& g, bool playerIsWhite)
    {
        std::string month = g.date.size() >= 7 ? g.date.substr(0, 7) : g.date;
        TrendPoint& tp    = buckets[month];
        tp.date           = month;
        ++tp.gamesPlayed;

        bool won  = (playerIsWhite && g.result == "1-0") ||
                    (!playerIsWhite && g.result == "0-1");
        bool draw = (g.result == "1/2-1/2");

        float prevWins = tp.winRate * (tp.gamesPlayed - 1);
        tp.winRate     = (prevWins + (won ? 1.0f : (draw ? 0.5f : 0.0f))) / tp.gamesPlayed;
    };

    for (GameRecord const& g : asWhite) process_game(g, true);
    for (GameRecord const& g : asBlack) process_game(g, false);

    std::vector<TrendPoint> trend;
    trend.reserve(buckets.size());
    for (auto const& kv : buckets)
        trend.push_back(kv.second);

    return trend;
}
