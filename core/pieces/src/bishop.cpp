/**
 * @file  bishop.cpp
 * @brief Bishop sources
 */

#include <assert.h>
#include <math.h>
#include "bishop.hpp"

Bishop::Bishop(bool _color, int _x, int _y)
    : Piece(_color, _x, _y)
{
}

bool
Bishop::checkMove(int _x, int _y, int& flags) const
{
    (void)flags;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The bishop moves along a diagonal, check the desired postion is reachable */
    if (sqrt(pow(x - _x, 2)) == sqrt(pow(y - _y, 2)))
    {
        return true;
    }

    return false;
}

bool
Bishop::move(int _x, int _y, int& flags)
{
    /* Check the king is able to move to the desired position */
    if (this->checkMove(_x, _y, flags))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
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
