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
Knight::move(int _x, int _y)
{
    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* Check the desired postion is reachable */
    if (((_x == x + 2) && (_y == y + 1)) || ((_x == x + 2) && (_y == y - 1)) || ((_x == x - 2) && (_y == y + 1)) || ((_x == x - 2) && (_y == y - 1))
        || ((_x == x + 1) && (_y == y + 2)) || ((_x == x + 1) && (_y == y - 2)) || ((_x == x - 1) && (_y == y + 2)) || ((_x == x - 1) && (_y == y - 2)))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

SquareValue
Knight::getValue(void) const
{
    return KnightValue;
}

void
Knight::print(std::ostream& os) const
{
    os << (alive ? (color ? " ♘ " : " ♞ ") : "   ");
}

std::ostream&
operator<<(std::ostream& os, Knight const& knight)
{
    knight.print(os);
    return os;
}