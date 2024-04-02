/**
 * @file  move.hpp
 * @brief Move pieces header
 */

#ifndef _MOVE_HPP_
#define _MOVE_HPP_

#include "piece.hpp"

using namespace std;

class Move
{
private:
    Piece* piece;  /**< The moving piece */
    int    xStart; /**< X position before move */
    int    yStart; /**< Y position before move */
    int    xEnd;   /**< X position after move */
    int    yEnd;   /**< Y position after move */

public:
    /**
     * @brief Construct a default rook
     */
    Move(Piece* _piece, int _xStart, int _yStart, int _xEnd, int _yEnd);

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
