/**
 * @file  board.hpp
 * @brief board header
 */

#ifndef _BOARD_HPP_
#define _BOARD_HPP_

#include <iostream>
#include <vector>

#include "piece.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "knight.hpp"
#include "pawn.hpp"
#include "queen.hpp"
#include "rook.hpp"
#include "square.hpp"

#define BOARD_SIZE_MAX (8)

class Board
{
protected:
    bool                isInitialize;
    Square*             board[BOARD_SIZE_MAX][BOARD_SIZE_MAX];
    std::vector<Piece*> whitePieces;
    std::vector<Piece*> blackPieces;

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
     * @brief Destroy the board
     */
    ~Board();

    /**
     * @brief Set the board with the default configuration
     */
    void startUp(void);

    /**
     * @brief Move a piece to the desired position
     * @param[in] piece Piece to move
     * @param[in] x X position to move
     * @param[in] y Y position to move
     * @param[out] flags Flags of the movement
     * @return true if the piece has moved, false otherwise
     */
    bool movePiece(Piece* piece, int x, int y, int& flags);

    /**
     * @brief Select a piece on the board
     * @param[out] piece Piece selected
     * @param[in] x X position where to select the piece
     * @param[in] y Y position where to select the piece
     * @return true if the piece is selected, false otherwise
     */
    bool selectPiece(Piece** piece, int x, int y);

    /**
     * @brief Check if a piece is able to reach the desired position with the given description
     * @param[out] piece Piece found (must be nullptr)
     * @param[in] pieceType Type of pieces to look for
     * @param[in] pieceColor Color of pieces to look for
     * @param[in] x X position to reach
     * @param[in] y Y position to reach
     * @param[in] flags Move flags
     * @param[in] extraFlag Extra flag for ambigous moves
     * @return true if a piece is found, false otherwise
     */
    bool searchPiece(Piece** piece, SquarePieceValue pieceType, SquarePieceColor pieceColor, int x, int y, int flags = 0, char extraFlag = ' ');

    /**
     * @brief Check if the square x, y is attacked a piece of the given color
     * @param[in] pieceColor Color of attacking pieces
     * @param[in] x X square position
     * @param[in] y Y square position
     * @return true if the square is attacked, false otherwise
     */
    bool isSquareAttacked(bool pieceColor, int x, int y);

    /**
     * @brief Check if king is check
     * @param[in] pieceColor King color to check
     * @return true if the king is check, false otherwise
     */
    bool isKingCheck(bool kingColor);

    /**
     * @brief Check if king is mate
     * @param[in] kingColor King color to check
     * @return true if the king is mate, false otherwise
     */
    bool isKingMate(bool kingColor);

    /**
     * @brief Check if king is in pat configuration
     * @param[in] kingColor King color to check
     * @return true if the king is pat, false otherwise
     */
    bool isKingPat(bool kingColor);

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
