/**
 * @file  statistics.hpp
 * @brief Chess game statistics and blunder detection
 */

#ifndef _STATISTICS_HPP_
#define _STATISTICS_HPP_

#include <string>
#include <vector>

#include "database.hpp"
#include "uci_engine.hpp"

/**
 * @brief A single move flagged as a mistake or blunder
 */
struct BlunderInfo
{
    int         moveNum;    /**< Half-move number (1-based) */
    std::string san;        /**< Move in SAN notation (from stored pgn_text, best-effort) */
    float       evalBefore; /**< Engine evaluation before the move (pawns) */
    float       evalAfter;  /**< Engine evaluation after the move (pawns) */
    float       drop;       /**< Absolute evaluation drop (always >= 0) */
    std::string severity;   /**< "mistake" (drop > 0.5) or "blunder" (drop > 2.0) */
};

/**
 * @brief Win/draw/loss statistics for an opening (by ECO code)
 */
struct OpeningStats
{
    std::string eco;         /**< ECO code (e.g. "B20") */
    std::string name;        /**< Opening name */
    int         gamesPlayed; /**< Total games with this ECO in the database */
    int         wins;        /**< White wins */
    int         draws;       /**< Draws */
    int         losses;      /**< Black wins */
    float       winRate;     /**< wins / gamesPlayed (0..1) */

    OpeningStats() : gamesPlayed(0), wins(0), draws(0), losses(0), winRate(0.0f) {}
};

/**
 * @brief One data point in a performance trend query
 */
struct TrendPoint
{
    std::string date;        /**< Date string (YYYY.MM.DD) */
    int         gamesPlayed; /**< Games played in this period */
    float       winRate;     /**< Win rate (0..1) for the queried player */
    float       blunderRate; /**< Average blunders per game (requires analyzeGame() data) */

    TrendPoint() : gamesPlayed(0), winRate(0.0f), blunderRate(0.0f) {}
};

/**
 * @brief High-level chess statistics computed over the database
 *
 * `analyzeGame()` stores evaluations in the `positions` table via
 * `Database::saveAnalysis()`. All other methods query stored data only.
 */
class Statistics
{
private:
    Database&   db;
    UCIEngine&  engine;

public:
    /**
     * @param[in,out] db     Open database (must outlive this object)
     * @param[in,out] engine Connected UCI engine (must outlive this object)
     */
    Statistics(Database& db, UCIEngine& engine);

    /**
     * @brief Evaluate every position in a game's main line and store results
     *
     * Fetches the game, replays each position, calls engine.analyze(), and
     * stores the result via Database::saveAnalysis().  Already-evaluated
     * positions are skipped.
     *
     * @param[in] gameId  Database game id
     * @param[in] depth   Engine search depth (default 18)
     * @return Number of positions evaluated, or -1 on error
     */
    int analyzeGame(int gameId, int depth = 18);

    /**
     * @brief Detect mistakes and blunders in an analysed game
     *
     * Requires that analyzeGame() has been called for this game first.
     *
     * @param[in] gameId            Database game id
     * @param[in] mistakeThreshold  Eval drop to flag as mistake (default 0.5 pawns)
     * @param[in] blunderThreshold  Eval drop to flag as blunder (default 2.0 pawns)
     * @return Vector of BlunderInfo, ordered by move number
     */
    std::vector<BlunderInfo> detectBlunders(int         gameId,
                                            float       mistakeThreshold = 0.5f,
                                            float       blunderThreshold = 2.0f) const;

    /**
     * @brief Aggregate win/draw/loss statistics for an ECO code prefix
     * @param[in] eco ECO prefix to match (e.g. "B" matches all B-codes, "B20" is exact)
     * @return OpeningStats for all matching games
     */
    OpeningStats getOpeningStats(std::string const& eco) const;

    /**
     * @brief Count how many games in the database reached a given position
     * @param[in] fen FEN string to look up
     * @return Number of matching position rows
     */
    int getPositionFrequency(std::string const& fen) const;

    /**
     * @brief Compute per-month win rate and blunder rate for a player
     * @param[in] player   Player name (exact)
     * @param[in] dateFrom Start date YYYY.MM.DD (inclusive, empty = no lower bound)
     * @param[in] dateTo   End date   YYYY.MM.DD (inclusive, empty = no upper bound)
     * @return Vector of TrendPoint ordered by date
     */
    std::vector<TrendPoint> getPerformanceTrend(std::string const& player,
                                                std::string const& dateFrom,
                                                std::string const& dateTo) const;
};

#endif /* _STATISTICS_HPP_ */
