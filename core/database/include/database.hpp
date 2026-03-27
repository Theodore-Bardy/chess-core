/**
 * @file  database.hpp
 * @brief SQLite database layer for chess game storage
 */

#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <string>
#include <vector>

#include <SQLiteCpp/SQLiteCpp.h>

#include "parser.hpp"

/**
 * @brief Filter parameters for game search queries
 */
struct GameFilter
{
    std::string white;    /**< White player name (substring match, empty = any) */
    std::string black;    /**< Black player name (substring match, empty = any) */
    std::string result;   /**< Game result: "1-0", "0-1", "1/2-1/2", empty = any */
    std::string dateFrom; /**< Earliest date inclusive (YYYY.MM.DD, empty = no lower bound) */
    std::string dateTo;   /**< Latest date inclusive (YYYY.MM.DD, empty = no upper bound) */
    std::string eco;      /**< ECO opening code prefix (e.g. "B", "B20", empty = any) */
    int         minElo;   /**< Minimum average Elo (0 = no lower bound) */
    int         maxElo;   /**< Maximum average Elo (0 = no upper bound) */

    GameFilter() : minElo(0), maxElo(0) {}
};

/**
 * @brief A game record returned from a database query
 */
struct GameRecord
{
    int         id;        /**< Primary key */
    std::string pgnText;   /**< Full PGN move text (without headers) */
    std::string result;    /**< "1-0", "0-1", "1/2-1/2", or "*" */
    std::string date;      /**< Date string from PGN header */
    std::string white;     /**< White player name */
    std::string black;     /**< Black player name */
    int         whiteElo;  /**< White Elo (0 if unknown) */
    int         blackElo;  /**< Black Elo (0 if unknown) */
    std::string eco;       /**< ECO opening code */
    std::string opening;   /**< Opening name */
    std::string event;     /**< Event name */
    std::string site;      /**< Site */
    std::string round;     /**< Round */

    GameRecord() : id(0), whiteElo(0), blackElo(0) {}
};

/**
 * @brief Aggregated statistics for a player
 */
struct PlayerStats
{
    std::string name;        /**< Player name */
    int         gamesPlayed; /**< Total games found in the database */
    int         wins;        /**< Games won */
    int         draws;       /**< Games drawn */
    int         losses;      /**< Games lost */

    PlayerStats() : gamesPlayed(0), wins(0), draws(0), losses(0) {}
};

/**
 * @brief SQLite-backed chess game database
 *
 * All public methods that write data are wrapped in a transaction.
 * The database file is created (with schema) if it does not exist.
 */
class Database
{
private:
    SQLite::Database db; /**< Underlying SQLiteCpp database handle */

    /**
     * @brief Create the schema if the tables do not exist yet
     */
    void initSchema();

    /**
     * @brief Insert or retrieve a player row; return the player id
     * @param[in] name Player name
     * @return Row id of the player
     */
    int upsertPlayer(std::string const& name);

    /**
     * @brief Convert a GameRecord to a GameMetadata struct for the PGN generator
     */
    static GameMetadata recordToMeta(GameRecord const& rec);

public:
    /**
     * @brief Open (or create) a database file
     * @param[in] path Path to the .db file
     */
    explicit Database(std::string const& path);

    /**
     * @brief Import all games from a PGN file into the database
     * @param[in] filePath Path to the PGN file
     * @return Number of games imported, or -1 on file error
     */
    int importPGN(std::string const& filePath);

    /**
     * @brief Search games matching the given filter
     * @param[in] filter Search criteria
     * @return Vector of matching GameRecord (metadata only, pgnText is empty)
     */
    std::vector<GameRecord> searchGames(GameFilter const& filter) const;

    /**
     * @brief Retrieve a single game by id (including full pgnText)
     * @param[in] id Database row id
     * @return GameRecord, or an empty record (id == 0) if not found
     */
    GameRecord getGame(int id) const;

    /**
     * @brief Find all games that pass through a given position
     * @param[in] fen FEN string of the position to search for
     * @return Vector of matching GameRecord (metadata only)
     */
    std::vector<GameRecord> getGamesByPosition(std::string const& fen) const;

    /**
     * @brief Store an engine evaluation for a position in a game
     * @param[in] gameId    Database game id
     * @param[in] moveNum   Half-move number (1-based)
     * @param[in] evalScore Evaluation in pawns (positive = white advantage)
     */
    void saveAnalysis(int gameId, int moveNum, float evalScore);

    /**
     * @brief Aggregate win/draw/loss statistics for a player
     * @param[in] name Player name (exact match)
     * @return PlayerStats struct
     */
    PlayerStats getPlayerStats(std::string const& name) const;
};

#endif /* _DATABASE_HPP_ */
