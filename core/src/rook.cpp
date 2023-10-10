/**
 * @file      rook.cpp
 * @brief     Rook sources
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#include "rook.hpp"

rook::rook(bool _color, int _x, int _y)
    : piece(_color, _x, _y)
{
    hasMoved = false;
}

rook::rook(bool _hasMoved, bool _isAlive, bool _color, int _x, int _y)
    : hasMoved(_hasMoved)
    , piece(_isAlive, _color, _x, _y)
{
}

rook::~rook()
{
}

bool
rook::castling(bool side)
{
    if (!hasMoved)
    {
        if (side)
        {
            x -= 2;
        }
        else
        {
            x += 3;
        }

        return true;
    }

    return false;
}

bool
rook::move(int _x, int _y)
{
    /* Check desired position exists */
    if ((_x < 8) || (_x > 1) || (_y < 8) || (_y > 1))
    {
        return false;
    }

    /* The rook moves along a rank, check the desired postion is reachable */
    if ((_x == x) || (_y == y))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

int
rook::getValue(void) const
{
    return ROOK_VALUE;
}

void
rook::print(std::ostream& os) const
{
    os << (isAlive ? " " : "*") << "R" << (color ? "w" : "b") << "[" << x << ";" << y << "]";
}

std::ostream&
operator<<(std::ostream& os, rook const& rook)
{
    rook.print(os);
    return os;
}
