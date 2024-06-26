/**
 * @file  knight.cpp
 * @brief Knight sources
 */

#include "knight.hpp"

Knight::Knight(bool _color, int _x, int _y)
    : Piece(_color, _x, _y)
{
}

Knight::Knight(bool _isAlive, bool _color, int _x, int _y)
    : Piece(_isAlive, _color, _x, _y)
{
}

Knight::~Knight()
{
}

bool
Knight::isAbleToMove(int _x, int _y, int flags, Square* board[8U][8U]) const
{
    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* Check the desired postion is reachable */
    if (((_x == x + 2) && (_y == y + 1)) || ((_x == x + 2) && (_y == y - 1)) || ((_x == x - 2) && (_y == y + 1)) || ((_x == x - 2) && (_y == y - 1))
        || ((_x == x + 1) && (_y == y + 2)) || ((_x == x + 1) && (_y == y - 2)) || ((_x == x - 1) && (_y == y + 2)) || ((_x == x - 1) && (_y == y - 2)))
    {
        xReturn = true;
    }

    /* Check desired position isn't a piece of the same color */
    if (xReturn && (board != nullptr))
    {
        xReturn = this->checkFinalOnMove(_x, _y, board);
    }

    return xReturn;
}

bool
Knight::move(int _x, int _y, int flags)
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
Knight::getValue(void) const
{
    return KnightValue;
}

void
Knight::print(std::ostream& os) const
{
    os << (color ? " ♘ " : " ♞ ");
}

std::ostream&
operator<<(std::ostream& os, Knight const& knight)
{
    knight.print(os);
    return os;
}