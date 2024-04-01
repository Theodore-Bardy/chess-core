/**
 * @file  board.hpp
 * @brief board header
 */

#ifndef _BOARD_HPP_
#define _BOARD_HPP_

#include <iostream>
#include <vector>

#include "piece.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "bishop.hpp"
#include "knight.hpp"
#include "rook.hpp"
#include "pawn.hpp"
#include "square.hpp"

#define BOARD_SIZE_MAX (8)

class Board
{
protected:
    bool                isInitialize;
    Square*             board[BOARD_SIZE_MAX][BOARD_SIZE_MAX];
    std::vector<Piece*> whitePieces;
    std::vector<Piece*> balckPieces;

    /* White pieces */
    King*   whiteKing;
    Queen*  whiteQueen;
    Bishop* whiteBishops[2];
    Knight* whiteKnights[2];
    Rook*   whiteRooks[2];
    Pawn*   whitePawns[8];

    /* Black pieces */
    King*   blackKing;
    Queen*  blackQueen;
    Bishop* blackBishops[2];
    Knight* blackKnights[2];
    Rook*   blackRooks[2];
    Pawn*   blackPawns[8];

public:
    /**
     * @brief Construct a new board
     */
    Board();

    /**
     * @brief Copy a new board
     */
    Board(Board const& boardToCopy);

    /**
     * @brief Destroy the board
     */
    ~Board();

    /**
     * @brief Set the board with the default configuration
     */
    void startUp();

    /**
     * @brief Move a piece to the desired position
     * @param[in] piece Piece to move
     * @param[in] x X position to move
     * @param[in] y Y position to move
     * @return true if the piece has moved, false otherwise
     */
    bool movePiece(Piece* piece, int x, int y);

    /**
     * @brief Select a piece on the board
     * @param[out] piece Piece selected
     * @param[in] x X position where to select the piece
     * @param[in] y Y position where to select the piece
     * @return true if the piece is selected, false otherwise
     */
    bool selectPiece(Piece** piece, int x, int y);

    /**
     * @brief Print board
     * @param[in, out] os Output stream
     */
    void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for board class
     * @param[in, out] os Output stream
     * @param[in] board Board object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Board const& board);
};

#endif /* _BOARD_HPP_ */
