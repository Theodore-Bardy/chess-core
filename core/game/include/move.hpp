/**
 * @file  move.hpp
 * @brief Move pieces header
 */

#ifndef _MOVE_HPP_
#define _MOVE_HPP_

#include <string>
#include "piece.hpp"

/**
 * @brief Optional flash on a move
 */
#define MOVE_FLAG_TAKE         (0x01) /**< (0b0000_0001) */
#define MOVE_FLAG_KING_CASTLE  (0x02) /**< (0b0000_0010) */
#define MOVE_FLAG_QUEEN_CASTLE (0x04) /**< (0b0000_0100) */
#define MOVE_FLAG_CHECK        (0x08) /**< (0b0000_1000) */
#define MOVE_FLAG_CHECK_MATE   (0x10) /**< (0b0001_0000) */
#define MOVE_FLAG_PROMOTION    (0x20) /**< (0b0010_0000) */

class Move
{
private:
    Piece* piece;     /**< The moving piece */
    int    xStart;    /**< X position before move */
    int    yStart;    /**< Y position before move */
    int    xEnd;      /**< X position after move */
    int    yEnd;      /**< Y position after move */
    int    flags;     /**< Move flags */
    char   extraFlag; /**< Extra flag used on pawn takes and ambigous moves */

public:
    /**
     * @brief Construct a default rook
     */
    Move(Piece* _piece, int _xStart, int _yStart, int _xEnd, int _yEnd, int _flags = 0, char _extraFlag = ' ');

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
     * @brief Print move
     * @return Move as string
     */
    virtual std::string print(void) const;

    /**
     * @brief Overloading the `<<` operator for move class
     * @param[in, out] os Output stream
     * @param[in] move Move object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Move const& move);
};

#endif /* _MOVE_HPP_ */
