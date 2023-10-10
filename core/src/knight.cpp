/**
 * @file      knight.cpp
 * @brief     Knight sources
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#include "knight.hpp"

knight::knight(bool _color, int _x, int _y)
    : piece(_color, _x, _y)
{
}

knight::knight(bool _isAlive, bool _color, int _x, int _y)
    : piece(_isAlive, _color, _x, _y)
{
}

knight::~knight()
{
}

bool
knight::move(int _x, int _y)
{
    /* Check desired position exists */
    if ((_x < 8) || (_x > 1) || (_y < 8) || (_y > 1))
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

int
knight::getValue(void) const
{
    return KNIGHT_VALUE;
}

void
knight::print(std::ostream& os) const
{
    os << (isAlive ? " " : "*") << "N" << (color ? "w" : "b") << "[" << x << ";" << y << "]";
}

std::ostream&
operator<<(std::ostream& os, knight const& knight)
{
    knight.print(os);
    return os;
}