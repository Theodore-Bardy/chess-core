/**
 * @file  parser.hpp
 * @brief Parser header
 */

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>

#include "game.hpp"

using namespace std;

/**
 * @brief PGN game metadata (header tags)
 */
struct GameMetadata
{
    std::string event;    /**< [Event "..."] */
    std::string site;     /**< [Site "..."] */
    std::string date;     /**< [Date "..."] */
    std::string round;    /**< [Round "..."] */
    std::string white;    /**< [White "..."] */
    std::string black;    /**< [Black "..."] */
    std::string result;   /**< [Result "..."] */
    int         whiteElo; /**< [WhiteElo "..."] */
    int         blackElo; /**< [BlackElo "..."] */
    std::string eco;      /**< [ECO "..."] */
    std::string opening;  /**< [Opening "..."] */

    GameMetadata() : whiteElo(0), blackElo(0) {}
};

class Parser
{
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
     * @brief Read a chess game from a PGN file
     * @param[in,out] game     Game object to populate
     * @param[in]     filePath Path to the PGN file
     * @param[out]    meta     Optional pointer to receive parsed header metadata
     * @return true if the file was read successfully, false otherwise
     */
    bool loadGame(Game& game, std::string const& filePath, GameMetadata* meta = nullptr);

    /**
     * @brief Write a chess game to a PGN file
     * @param[in] game     Game to serialize (traverses the full game tree)
     * @param[in] filePath Path to the output PGN file
     * @param[in] meta     Optional metadata to write as PGN headers
     * @return true if the file was written successfully, false otherwise
     */
    bool saveGame(Game const& game, std::string const& filePath, GameMetadata const* meta = nullptr);
};

#endif /* _PARSER_HPP_ */
