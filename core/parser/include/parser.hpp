/**
 * @file  parser.hpp
 * @brief Parser header
 */

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "piece.hpp"
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class Parser
{
private:
    vector<string> gameDescription;

public:
    /**
     * @brief Default constructor
     */
    Parser() = default;

    /**
     * @brief Default destructor
     */
    ~Parser() = default;

    /**
     * @brief Read a Chess game from a PGN files format
     * @param[in] filePath Path to the file
     * @param[in] fileName Name of the file
     * @return true if file is found, false otherwise
     */
    bool loadGame(string filePath, string fileName);

    /**
     * @brief Save a Chess game from to a PGN files format
     * @param[in] filePath Path to the file
     * @param[in] fileName Name of the file
     * @return true if file is created, false otherwise
     */
    bool saveGame(string filePath, string fileName);

    /**
     * @brief Returns the gameDescription attribute
     * @return gameDescription attribute
     */
    vector<string> getGameDescription(void)
    {
        return gameDescription;
    }
};

#endif /* _PARSER_HPP_ */
