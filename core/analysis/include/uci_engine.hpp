/**
 * @file  uci_engine.hpp
 * @brief UCI chess engine interface (Stockfish or any UCI-compatible engine)
 */

#ifndef _UCI_ENGINE_HPP_
#define _UCI_ENGINE_HPP_

#include <string>
#include <vector>

#include "process.hpp"

/**
 * @brief Result returned by a Stockfish analysis call
 */
struct EngineResult
{
    std::string              bestMove;   /**< Best move in UCI notation (e.g. "e2e4") */
    std::string              ponderMove; /**< Ponder move (may be empty) */
    float                    score;      /**< Evaluation in pawns (positive = white advantage) */
    int                      depth;      /**< Search depth reached */
    std::vector<std::string> pvLine;     /**< Principal variation (list of UCI move strings) */

    EngineResult() : score(0.0f), depth(0) {}
};

/**
 * @brief Controls a UCI chess engine as a child process
 *
 * Typical usage:
 * @code
 *   UCIEngine engine;
 *   engine.connect("/usr/bin/stockfish");
 *   engine.setPosition("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
 *   EngineResult r = engine.analyze(20);
 *   std::cout << r.bestMove << " score=" << r.score << "\n";
 *   engine.shutdown();
 * @endcode
 */
class UCIEngine
{
private:
    Process process;
    bool    connected;

    /**
     * @brief Send a raw command string to the engine
     */
    bool sendCommand(std::string const& cmd);

    /**
     * @brief Block until a line starting with @p prefix is received
     * @param[in]  prefix     Expected line prefix
     * @param[out] lineOut    Full received line (optional, may be nullptr)
     * @param[in]  timeoutMs  Per-line read timeout
     * @return true if the expected line was received before timeout
     */
    bool waitFor(std::string const& prefix, std::string* lineOut = nullptr,
                 int timeoutMs = 5000);

public:
    UCIEngine();
    ~UCIEngine();

    /**
     * @brief Spawn the engine and complete the UCI handshake
     * @param[in] enginePath Full path to the engine binary
     * @return true if the engine is ready to receive commands
     */
    bool connect(std::string const& enginePath);

    /**
     * @brief Set the position to analyse
     * @param[in] fen Standard FEN string
     */
    void setPosition(std::string const& fen);

    /**
     * @brief Run a fixed-depth search on the current position
     * @param[in] depth Search depth in plies
     * @return EngineResult with best move, score, and PV line
     */
    EngineResult analyze(int depth = 20);

    /**
     * @brief Send "quit" and wait for the process to exit
     */
    void shutdown();

    /**
     * @brief Returns true if the engine process is running
     */
    bool isConnected() const { return connected; }
};

#endif /* _UCI_ENGINE_HPP_ */
