/**
 * @file  queen.cpp
 * @brief Queen soucres
 */

#include "queen.hpp"
#include "math.h"

Queen::Queen(bool _color)
    : Piece(_color, 0, 0)
{
    isAlive = true;

    if (_color)
    {
        /* Default position for white queen */
        x = QUEEN_WHITE_DEFAULT_X;
        y = QUEEN_WHITE_DEFAULT_Y;
    }
    else
    {
        /* Default position for black queen */
        x = QUEEN_BLACK_DEFAULT_X;
        y = QUEEN_BLACK_DEFAULT_Y;
    }
}

Queen::Queen(bool _isAlive, bool _color, int _x, int _y)
    : Piece(_isAlive, _color, _x, _y)
{
}

Queen::~Queen()
{
}

bool
Queen::move(int _x, int _y)
{
    /* Check desired position exists and it is not the current position */
    if ((_x > 8) || (_x < 1) || (_y > 8) || (_y < 1) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The queen moves in any direction at any distance, check the desired postion
   * is reachable */
    if ((_x == x) || (_y == y) || (sqrt(pow(x - _x, 2)) == sqrt(pow(y - _y, 2))))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

int
Queen::getValue(void) const
{
    return QUEEN_VALUE;
}

void
Queen::print(std::ostream& os) const
{
    os << (isAlive ? (color ? " ♕ " : " ♛ ") : "   ");
}

std::ostream&
operator<<(std::ostream& os, Queen const& queen)
{
    queen.print(os);
    return os;
}
