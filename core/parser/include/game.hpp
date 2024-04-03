/**
 * @file  game.hpp
 * @brief Chess game header
 */

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <string>
#include <vector>

#include "board.hpp"
#include "move.hpp"

using namespace std;

class Game
{
private:
    Board              gameBoard;
    std::vector<Move*> moves;
    int                moveIndex;

public:
    /**
     * @brief Default constructor
     */
    Game();

    /**
     * @brief Default destructor
     */
    ~Game();

    /**
     * @brief Add a move into the moves vector
     * @param[in] xStart X position where select the piece
     * @param[in] yStart Y position where select the piece
     * @param[in] xEnd X position where move the piece
     * @param[in] yEnd Y position where move the piece
     * @param[in] flags Flags of the movement
     * @return true if the move is possible, false otherwise
     */
    bool addMove(int xStart, int yStart, int xEnd, int yEnd, int flags);

    /**
     * @brief Add a move into the moves vector from a piece description
     * @param[in] pieceType Piece type to move
     * @param[in] pieceColor Piece color to move
     * @param[in] xEnd X position where move the piece
     * @param[in] yEnd Y position where move the piece
     * @param[in] flags Flags of the movement
     * @return true if the move is possible, false otherwise
     */
    bool addMoveFromPieceDesc(SquareValue pieceType, SquarePieceColor pieceColor, int xEnd, int yEnd, int flags);

    /**
     * @brief Returns the move index
     * @return The move index attribute
     */
    int getMoveIndex(void) const
    {
        return moveIndex;
    }

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
