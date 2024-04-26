/**
 * @file  queen.cpp
 * @brief Queen soucres
 */

#include <assert.h>
#include <math.h>
#include "queen.hpp"

Queen::Queen(bool _color)
    : Piece(_color, 0, 0)
{
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

Queen::Queen(bool _color, int _x, int _y)
    : Piece(_color, _x, _y)
{
}

bool
Queen::checkMove(int _x, int _y, int& flags) const
{
    (void)flags;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The queen moves in any direction at any distance, check the desired postion
   * is reachable */
    if ((_x == x) || (_y == y) || (sqrt(pow(x - _x, 2)) == sqrt(pow(y - _y, 2))))
    {
        return true;
    }

    return false;
}

bool
Queen::move(int _x, int _y, int& flags)
{
    /* Check the queen is able to move to the desired position */
    if (this->checkMove(_x, _y, flags))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

void
Queen::print(std::ostream& os) const
{
    os << (color ? " ♕ " : " ♛ ");
}

std::ostream&
operator<<(std::ostream& os, Queen const& queen)
{
    queen.print(os);
    return os;
}
