/**
 * @file      pawn.cpp
 * @brief     Pawn sources
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#include "pawn.hpp"

pawn::pawn(bool _color, int _x, int _y)
    : piece(_color, _x, _y)
{
    hasMoved = false;
}

pawn::pawn(bool _hasMoved, bool _isAlive, bool _color, int _x, int _y)
    : hasMoved(_hasMoved)
    , piece(_isAlive, _color, _x, _y)
{
}

pawn::~pawn()
{
}

void
pawn::promotion()
{
    // TODO
}

bool
pawn::move(int _x, int _y)
{
    /* Check desired position exists */
    if ((_x < 8) || (_x > 1) || (_y < 8) || (_y > 1))
    {
        return false;
    }

    /* Check the desired postion is reachable */
    // TODO

    return false;
}

int
pawn::getValue(void) const
{
    return PAWN_VALUE;
}

void
pawn::print(std::ostream& os) const
{
    os << (isAlive ? " " : "*") << "P" << (color ? "w" : "b") << "[" << x << ";" << y << "]";
}

std::ostream&
operator<<(std::ostream& os, pawn const& pawn)
{
    pawn.print(os);
    return os;
}
