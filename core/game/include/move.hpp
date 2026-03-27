/**
 * @file  move.hpp
 * @brief Move pieces header
 */

#ifndef _MOVE_HPP_
#define _MOVE_HPP_

#include "piece.hpp"

using namespace std;

/**
 * @brief Optional flash on a move
 */
#define MOVE_FLAG_TAKE         (0x01) /**< (0b0000_0001) Capture move */
#define MOVE_FLAG_KING_CASTLE  (0x02) /**< (0b0000_0010) Kingside castling */
#define MOVE_FLAG_QUEEN_CASTLE (0x04) /**< (0b0000_0100) Queenside castling */
#define MOVE_FLAG_CHECK        (0x08) /**< (0b0000_1000) Move gives check */
#define MOVE_FLAG_CHECK_MATE   (0x10) /**< (0b0001_0000) Move gives checkmate */
#define MOVE_FLAG_EN_PASSANT   (0x20) /**< (0b0010_0000) En passant pawn capture */
#define MOVE_FLAG_PROMOTION    (0x40) /**< (0b0100_0000) Pawn promotion */

class Move
{
private:
    Piece*           piece;          /**< The moving piece */
    int              xStart;         /**< X position before move */
    int              yStart;         /**< Y position before move */
    int              xEnd;           /**< X position after move */
    int              yEnd;           /**< Y position after move */
    int              flags;          /**< Move flags */
    char             extraFlag;      /**< Extra flag used on pawn takes and ambigous moves */
    SquarePieceValue promotionPiece; /**< Piece type chosen on pawn promotion (Empty = no promotion) */

public:
    /**
     * @brief Construct a move
     */
    Move(Piece* _piece, int _xStart, int _yStart, int _xEnd, int _yEnd, int _flags = 0, char _extraFlag = ' ', SquarePieceValue _promotionPiece = SquarePieceValue::Empty);

    /**
     * @brief Returns the moving piece
     * @return The moving piece
     */
    Piece* getPiece(void) const
    {
        return piece;
    }

    /**
     * @brief Returns the X position before move
     * @return X position before move
     */
    int getXStart(void) const
    {
        return xStart;
    }

    /**
     * @brief Returns the Y position before move
     * @return Y position before move
     */
    int getYStart(void) const
    {
        return yStart;
    }

    /**
     * @brief Returns the X position after move
     * @return X position after move
     */
    int getXEnd(void) const
    {
        return xEnd;
    }

    /**
     * @brief Returns the Y position after move
     * @return Y position after move
     */
    int getYEnd(void) const
    {
        return yEnd;
    }

    /**
     * @brief Returns the move flags
     * @return Move flags
     */
    int getFlags(void) const
    {
        return flags;
    }

    /**
     * @brief Returns the extra disambiguation flag
     * @return File ('a'-'h') or rank ('1'-'8') used to disambiguate, ' ' if none
     */
    char getExtraFlag(void) const
    {
        return extraFlag;
    }

    /**
     * @brief Returns the promotion piece type
     * @return Promotion piece type (Empty if not a promotion)
     */
    SquarePieceValue getPromotionPiece(void) const
    {
        return promotionPiece;
    }

    /**
     * @brief Print move
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for move class
     * @param[in, out] os Output stream
     * @param[in] move Move object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Move const& move);
};

#endif /* _MOVE_HPP_ */
