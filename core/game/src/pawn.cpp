/**
 * @file  pawn.cpp
 * @brief Pawn sources
 */

#include "pawn.hpp"

Pawn::Pawn(bool _color, int _x)
    : Piece(_color, _x, 0)
{
    hasMoved = false;
    if (_color)
    {
        y = PAWN_WHITE_DEFAULT_Y;
    }
    else
    {
        y = PAWN_BLACK_DEFAULT_Y;
    }
}

Pawn::Pawn(bool _hasMoved, bool _isAlive, bool _color, int _x, int _y)
    : hasMoved(_hasMoved)
    , Piece(_isAlive, _color, _x, _y)
{
    /* Ensure the pawn have not moved */
    if ((color && (_y != PAWN_WHITE_DEFAULT_Y)) || (!color && (_y != PAWN_BLACK_DEFAULT_Y)))
    {
        hasMoved = true;
    }
}

Pawn::~Pawn()
{
}

void
Pawn::promotion()
{
    // TODO
}

bool
Pawn::move(int _x, int _y)
{
    /* Check desired position exists and it is not the current position */
    if ((_x > 8) || (_x < 1) || (_y > 8) || (_y < 1) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* Check the desired postion is reachable */
    if (color)
    {
        /* White pawn must increase Y at each move */
        if ((_y > y) && ((_y == y + 1) || (!hasMoved && (_y == y + 2))))
        {
            x = _x;
            y = _y;
            return true;
        }
    }
    else
    {
        /* Black pawn must decrease Y at each move */
        if ((_y < y) && ((_y == y - 1) || (!hasMoved && (_y == y - 2))))
        {
            x = _x;
            y = _y;
            return true;
        }
    }

    return false;
}

int
Pawn::getValue(void) const
{
    return PAWN_VALUE;
}

void
Pawn::print(std::ostream& os) const
{
    os << (isAlive ? (color ? " ♙ " : " ♟ ") : "   ");
}

std::ostream&
operator<<(std::ostream& os, Pawn const& pawn)
{
    pawn.print(os);
    return os;
}
