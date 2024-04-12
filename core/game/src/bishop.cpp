/**
 * @file  bishop.cpp
 * @brief Bishop sources
 */

#include "math.h"
#include "bishop.hpp"

Bishop::Bishop(bool _color, int _x, int _y)
    : Piece(_color, _x, _y)
{
}

Bishop::Bishop(bool _isAlive, bool _color, int _x, int _y)
    : Piece(_isAlive, _color, _x, _y)
{
}

Bishop::~Bishop()
{
}

bool
Bishop::isAbleToMove(int _x, int _y, int flags, Square* board[8U][8U]) const
{
    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The bishop moves along a diagonal, check the desired postion is reachable */
    if (sqrt(pow(x - _x, 2)) == sqrt(pow(y - _y, 2)))
    {
        xReturn = true;
    }

    /* Check if there is piece between current and desired position */
    if (xReturn && (board != nullptr))
    {
        xReturn = this->checkWayOnMove(_x, _y, board);
        if (xReturn)
        {
            xReturn = this->checkFinalOnMove(_x, _y, board);
        }
    }

    return xReturn;
}

bool
Bishop::move(int _x, int _y, int flags)
{
    /* Check the king is able to move to the desired position */
    if (this->isAbleToMove(_x, _y, flags))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

SquarePieceValue
Bishop::getValue(void) const
{
    return BishopValue;
}

void
Bishop::print(std::ostream& os) const
{
    os << (color ? " ♗ " : " ♝ ");
}

std::ostream&
operator<<(std::ostream& os, Bishop const& bishop)
{
    bishop.print(os);
    return os;
}
