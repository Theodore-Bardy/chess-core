/**
 * @file  rook.cpp
 * @brief Rook sources
 */

#include "rook.hpp"

Rook::Rook(bool _color, int _x, int _y)
    : Piece(_color, _x, _y)
{
    hasMoved = false;
}

Rook::Rook(bool _hasMoved, bool _isAlive, bool _color, int _x, int _y)
    : hasMoved(_hasMoved)
    , Piece(_isAlive, _color, _x, _y)
{
}

Rook::~Rook()
{
}

bool
Rook::castling(bool side)
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
Rook::isAbleToMove(int _x, int _y) const
{
    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The rook moves along a rank, check the desired postion is reachable */
    if ((_x == x) || (_y == y))
    {
        return true;
    }
    return false;
}

bool
Rook::move(int _x, int _y)
{
    /* Check the king is able to move to the desired position */
    if (this->isAbleToMove(_x, _y))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

SquareValue
Rook::getValue(void) const
{
    return RookValue;
}

void
Rook::print(std::ostream& os) const
{
    os << (alive ? (color ? " ♖ " : " ♜ ") : "   ");
}

std::ostream&
operator<<(std::ostream& os, Rook const& rook)
{
    rook.print(os);
    return os;
}
