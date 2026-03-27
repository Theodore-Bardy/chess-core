/**
 * @file  board.hpp
 * @brief board header
 */

#ifndef _BOARD_HPP_
#define _BOARD_HPP_

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "move.hpp"
#include "piece.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "bishop.hpp"
#include "knight.hpp"
#include "rook.hpp"
#include "pawn.hpp"
#include "square.hpp"

#define BOARD_SIZE_MAX (8)

/**
 * @brief Result of a chess game
 */
enum GameResult
{
    InProgress,    /**< Game is still ongoing */
    WhiteWins,     /**< White wins by checkmate */
    BlackWins,     /**< Black wins by checkmate */
    DrawStalemate, /**< Draw by stalemate */
    DrawFiftyMove, /**< Draw by fifty-move rule */
    DrawThreefold  /**< Draw by threefold repetition */
};

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

    /* En passant target square (-1 means none) */
    int enPassantX;
    int enPassantY;

    /* Draw condition tracking */
    int                        halfMoveClock;   /**< Increments each half-move; resets on capture or pawn move */
    std::map<std::string, int> positionHistory; /**< Maps position hash to occurrence count */

private:
    /**
     * @brief Set named piece pointers from the piece vectors.
     *        Must be called after copy construction to fix dangling pointers.
     */
    void setupNamedPointers();

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
     * @param[in] flags Flags of the movement
     * @param[in] promotionPiece Piece type chosen on pawn promotion (default: queen)
     * @return true if the piece has moved, false otherwise
     */
    bool movePiece(Piece* piece, int x, int y, int flags, SquarePieceValue promotionPiece = SquarePieceValue::QueenValue);

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
    bool checkMove(Piece** piece, SquarePieceValue pieceType, SquarePieceColor pieceColor, int x, int y, int flags = 0, char extraFlag = ' ');

    /**
     * @brief Check if the square x, y is attacked by a piece of the given color
     * @param[in] pieceColor Color of attacking pieces
     * @param[in] x X square position
     * @param[in] y Y square position
     * @return true if the square is attacked, false otherwise
     */
    bool isSquareAttacked(bool pieceColor, int x, int y);

    /**
     * @brief Check if the king of the given color is in check
     * @param[in] color true = white, false = black
     * @return true if the king is in check, false otherwise
     */
    bool isInCheck(bool color);

    /**
     * @brief Check if a move is legal (does not leave own king in check)
     * @param[in] piece Piece to move
     * @param[in] xEnd Destination X
     * @param[in] yEnd Destination Y
     * @param[in] flags Move flags
     * @return true if the move is legal, false otherwise
     */
    bool isMoveLegal(Piece* piece, int xEnd, int yEnd, int flags);

    /**
     * @brief Generate all legal moves for pieces of the given color
     * @param[in] color true = white, false = black
     * @return Vector of all legal Move objects
     */
    std::vector<Move> getLegalMoves(bool color);

    /**
     * @brief Determine the current game result for the side to move
     * @param[in] colorToMove true = white to move, false = black to move
     * @return GameResult enum value
     */
    GameResult getGameResult(bool colorToMove);

    /**
     * @brief Serialize the current position to a FEN string
     * @param[in] colorToMove true = white to move, false = black to move
     * @param[in] fullMoveNumber Full-move counter (1-based, increments after Black's move)
     * @return Standard FEN string
     */
    std::string getFEN(bool colorToMove, int fullMoveNumber = 1) const;

    /**
     * @brief Compute a hash string representing the current position
     *        (piece layout + castling rights + en passant + side to move)
     * @param[in] colorToMove true = white to move, false = black to move
     * @return Hash string used for threefold repetition detection
     */
    std::string computeHash(bool colorToMove) const;

    /**
     * @brief Record the current position in the history and update the half-move clock
     * @param[in] colorToMove Side that just moved (for hash computation)
     * @param[in] isCapture true if the move was a capture
     * @param[in] isPawnMove true if the move was a pawn move
     */
    void updateDrawTracking(bool colorToMove, bool isCapture, bool isPawnMove);

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
