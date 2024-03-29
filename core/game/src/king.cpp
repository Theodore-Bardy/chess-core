/**
 * @file  king.cpp
 * @brief King sources
 */

#include "king.hpp"

King::King(bool _color)
    : Piece(_color, 0, 0)
{
    isCheck  = false;
    hasMoved = false;

    if (_color)
    {
        /* Default position for white king */
        x = KING_WHITE_DEFAULT_X;
        y = KING_WHITE_DEFAULT_Y;
    }
    else
    {
        /* Default position for black king */
        x = KING_BLACK_DEFAULT_X;
        y = KING_BLACK_DEFAULT_Y;
    }
}

King::King(bool _isCheck, bool _hasMoved, bool _isAlive, bool _color, int _x, int _y)
    : isCheck(_isCheck)
    , hasMoved(_hasMoved)
    , Piece(_isAlive, _color, _x, _y)
{
    /* Ensure king have not moved */
    if ((color && ((_x != KING_WHITE_DEFAULT_X) || (_y != KING_WHITE_DEFAULT_Y))) || (!color && ((_x != KING_BLACK_DEFAULT_X) || (_y != KING_BLACK_DEFAULT_Y))))
    {
        hasMoved = true;
    }
}

King::~King()
{
}

bool
King::castling(bool side)
{
    if (!hasMoved)
    {
        // TODO check is the king is check on the way
        if (side)
        {
            x += 2;
        }
        else
        {
            x -= 2;
        }

        return true;
    }

    return false;
}

bool
King::move(int _x, int _y)
{
    /* Check desired position exists */
    if ((_x < 8) || (_x > 1) || (_y < 8) || (_y > 1))
    {
        return false;
    }

    /* The king moves one square in any direction, check the desired postion is
   * reachable */
    if ((_x <= (x + 1)) && (_x >= (x - 1)) && (_y <= (y + 1)) && (_y >= (y - 1)))
    {
        // TODO check is the king is check on the new square
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

int
King::getValue(void) const
{
    return KING_VALUE;
}

void
King::print(std::ostream& os) const
{
    os << (isAlive ? " " : "*") << "K" << (color ? "w" : "b") << "[" << x << ";" << y << "]";
}

std::ostream&
operator<<(std::ostream& os, King const& king)
{
    king.print(os);
    return os;
}
