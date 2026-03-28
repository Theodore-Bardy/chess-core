/**
 * @file  uci_engine.cpp
 * @brief UCI engine interface implementation
 */

#include "uci_engine.hpp"

#include <sstream>

UCIEngine::UCIEngine()
    : connected(false)
{}

UCIEngine::~UCIEngine()
{
    if (connected)
        shutdown();
}

bool UCIEngine::sendCommand(std::string const& cmd)
{
    return process.writeLine(cmd);
}

bool UCIEngine::waitFor(std::string const& prefix, std::string* lineOut, int timeoutMs)
{
    std::string line;
    while (process.readLine(line, timeoutMs))
    {
        if (line.find(prefix) == 0)
        {
            if (lineOut) *lineOut = line;
            return true;
        }
    }
    return false;
}

bool UCIEngine::connect(std::string const& enginePath)
{
    if (!process.start(enginePath))
        return false;

    /* UCI handshake */
    if (!sendCommand("uci"))           { process.terminate(); return false; }
    if (!waitFor("uciok"))             { process.terminate(); return false; }
    if (!sendCommand("isready"))       { process.terminate(); return false; }
    if (!waitFor("readyok"))           { process.terminate(); return false; }

    connected = true;
    return true;
}

void UCIEngine::setPosition(std::string const& fen)
{
    sendCommand("position fen " + fen);
}

EngineResult UCIEngine::analyze(int depth)
{
    EngineResult result;

    sendCommand("go depth " + std::to_string(depth));

    std::string line;
    /* Collect info lines until "bestmove" */
    while (process.readLine(line, 30000 /* 30 s max */))
    {
        if (line.find("bestmove") == 0)
        {
            /* bestmove e2e4 ponder e7e5 */
            std::istringstream ss(line);
            std::string token;
            ss >> token; /* "bestmove" */
            ss >> result.bestMove;
            if (ss >> token && token == "ponder")
                ss >> result.ponderMove;
            break;
        }

        if (line.find("info") == 0)
        {
            /* Parse: info depth N score cp N pv move1 move2 ... */
            std::istringstream ss(line);
            std::string token;
            int parsedDepth = 0;
            float parsedScore = 0.0f;
            bool isMate = false;
            std::vector<std::string> pv;

            while (ss >> token)
            {
                if (token == "depth")
                {
                    ss >> parsedDepth;
                }
                else if (token == "score")
                {
                    std::string scoreType;
                    ss >> scoreType;
                    int val = 0;
                    ss >> val;
                    if (scoreType == "cp")
                        parsedScore = val / 100.0f;
                    else if (scoreType == "mate")
                    {
                        isMate = true;
                        parsedScore = (val > 0) ? 999.0f : -999.0f;
                    }
                }
                else if (token == "pv")
                {
                    std::string mv;
                    pv.clear();
                    while (ss >> mv)
                        pv.push_back(mv);
                    break;
                }
            }

            /* Keep the deepest info line */
            if (parsedDepth >= result.depth)
            {
                result.depth  = parsedDepth;
                result.score  = parsedScore;
                result.pvLine = pv;
                (void)isMate;
            }
        }
    }

    return result;
}

void UCIEngine::shutdown()
{
    sendCommand("quit");
    process.terminate();
    connected = false;
}
