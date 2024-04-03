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
King::isAbleToMove(int _x, int _y) const
{
    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The king moves one square in any direction, check the desired postion is
   * reachable */
    if ((_x <= (x + 1)) && (_x >= (x - 1)) && (_y <= (y + 1)) && (_y >= (y - 1)))
    {
        // TODO check is the king is check on the new square
        return true;
    }

    return false;
}

bool
King::move(int _x, int _y)
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
King::getValue(void) const
{
    return KingValue;
}

void
King::print(std::ostream& os) const
{
    os << (alive ? (color ? " ♔ " : " ♚ ") : "   ");
}

std::ostream&
operator<<(std::ostream& os, King const& king)
{
    king.print(os);
    return os;
}
