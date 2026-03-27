/**
 * @file  game.hpp
 * @brief Chess game header
 */

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <string>
#include <vector>

#include "board.hpp"
#include "game_tree.hpp"
#include "move.hpp"

using namespace std;

class Game
{
private:
    Board    gameBoard;     /**< Working board — always matches tree cursor's board */
    GameTree tree;          /**< Full game tree including all variations */
    bool     colorToMove;   /**< true = white to move, false = black */
    int      fullMoveNumber; /**< Full-move counter (1-based, increments after Black's move) */

public:
    /**
     * @brief Default constructor — sets up the starting position
     */
    Game();

    /**
     * @brief Default destructor
     */
    ~Game();

    /**
     * @brief Add a move by piece coordinates
     * @param[in] xStart X position where to select the piece
     * @param[in] yStart Y position where to select the piece
     * @param[in] xEnd   X destination
     * @param[in] yEnd   Y destination
     * @param[in] flags  Move flags
     * @param[in] extraFlag Extra flag for ambiguous moves
     * @param[in] promotionPiece Piece type on pawn promotion (default: queen)
     * @return true if the move was applied, false otherwise
     */
    bool addMove(int xStart, int yStart, int xEnd, int yEnd, int flags = 0, char extraFlag = ' ',
                 SquarePieceValue promotionPiece = SquarePieceValue::QueenValue);

    /**
     * @brief Add a move by piece type description (used by PGN parser)
     * @param[in] pieceType   Piece type to move
     * @param[in] pieceColor  Piece color to move
     * @param[in] xEnd        X destination
     * @param[in] yEnd        Y destination
     * @param[in] flags       Move flags
     * @param[in] extraFlag   Extra flag for ambiguous moves
     * @param[in] promotionPiece Piece type on pawn promotion (default: queen)
     * @return true if the move was applied, false otherwise
     */
    bool checkMove(SquarePieceValue pieceType, SquarePieceColor pieceColor, int xEnd, int yEnd,
                   int flags = 0, char extraFlag = ' ',
                   SquarePieceValue promotionPiece = SquarePieceValue::QueenValue);

    /**
     * @brief Move the cursor forward by one ply (main line by default)
     * @param[in] variation  Child index to follow (0 = main line)
     * @return true if navigation succeeded
     */
    bool forward(int variation = 0);

    /**
     * @brief Move the cursor backward by one ply
     * @return true if navigation succeeded (was not already at root)
     */
    bool back();

    /**
     * @brief Save cursor position and step back one ply.
     *        Called by the PGN parser before parsing a parenthesised variation.
     */
    void startVariation();

    /**
     * @brief Restore the cursor saved by the matching startVariation().
     *        Called by the PGN parser after the closing ')'.
     */
    void endVariation();

    /**
     * @brief Returns the number of half-moves played (depth in main line)
     */
    int getMoveIndex(void) const
    {
        return tree.getDepth();
    }

    /**
     * @brief Returns the game tree (read-only) — used by the PGN generator
     */
    GameTree const& getTree() const { return tree; }

    /**
     * @brief Returns true if it is white's turn to move
     */
    bool getColorToMove() const { return colorToMove; }

    /**
     * @brief Returns the current FEN string
     */
    std::string getFEN() const;

    /**
     * @brief Set the comment on the most recently added move (current cursor node)
     * @param[in] comment Annotation text
     */
    void setComment(std::string const& comment) { tree.setCursorComment(comment); }

    /**
     * @brief Print game
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for game class
     * @param[in, out] os Output stream
     * @param[in] game Game object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Game const& game);
};

#endif /* _GAME_HPP_ */
