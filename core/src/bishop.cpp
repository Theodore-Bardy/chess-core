/**
 * @file      bishop.cpp
 * @brief     Bishop sources
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#include "math.h"
#include "bishop.hpp"

bishop::bishop(bool _color, int _x, int _y)
    : piece(_color, _x, _y)
{
}

bishop::bishop(bool _isAlive, bool _color, int _x, int _y)
    : piece(_isAlive, _color, _x, _y)
{
}

bishop::~bishop()
{
}

bool
bishop::move(int _x, int _y)
{
    /* Check desired position exists */
    if ((_x < 8) || (_x > 1) || (_y < 8) || (_y > 1))
    {
        return false;
    }

    /* The bishop moves along a diagonal, check the desired postion is reachable */
    if (sqrt(pow(x - _x, 2)) == sqrt(pow(y - _y, 2)))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

int
bishop::getValue(void) const
{
    return BISHOP_VALUE;
}

void
bishop::print(std::ostream& os) const
{
    os << (isAlive ? " " : "*") << "B" << (color ? "w" : "b") << "[" << x << ";" << y << "]";
}

std::ostream&
operator<<(std::ostream& os, bishop const& bishop)
{
    bishop.print(os);
    return os;
}
