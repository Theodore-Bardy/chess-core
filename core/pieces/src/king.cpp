/**
 * @file  king.cpp
 * @brief King sources
 */

#include <assert.h>
#include "king.hpp"

King::King(bool _color)
    : Piece(_color, 0, 0)
    , isCheck(false)
    , isMate(false)
    , isPat(false)
    , hasMoved(false)
{
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

bool
King::castle(bool side, Rook& rook)
{
    if (!hasMoved && !isCheck && !isMate && !isPat && (((KING_CASTLE == side) && rook.isKingSideRook()) || ((QUEEN_CASTLE == side) && !rook.isKingSideRook())))
    {
        if (rook.castle())
        {
            x        = (side) ? KING_KING_CASTLE_X : KING_QUEEN_CASTLE_X;
            hasMoved = true;
            return true;
        }
    }

    return false;
}

bool
King::checkMove(int _x, int _y, int& flags) const
{
    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* Check for king castle move */
    if (!hasMoved && !isCheck && !isMate && !isPat && (_y == y) && (_x == KING_KING_CASTLE_X))
    {
        flags |= MOVE_FLAG_KING_CASTLE;
        return true;
    }
    /* Check for queen castle move */
    else if (!hasMoved && !isCheck && !isMate && !isPat && (_y == y) && (_x == KING_QUEEN_CASTLE_X))
    {
        flags |= MOVE_FLAG_QUEEN_CASTLE;
        return true;
    }
    /* The king moves one square in any direction, check the desired postion is reachable */
    else if ((_x <= (x + 1)) && (_x >= (x - 1)) && (_y <= (y + 1)) && (_y >= (y - 1)))
    {
        return true;
    }

    return false;
}

bool
King::move(int _x, int _y, int& flags)
{
    /* Make sure flags equal 0 */
    flags = 0;

    /* Check the king is able to move to the desired position */
    if (this->checkMove(_x, _y, flags) && (flags == 0))
    {
        x        = _x;
        y        = _y;
        hasMoved = true;
        return true;
    }

    return false;
}

void
King::print(std::ostream& os) const
{
    os << (color ? " ♔ " : " ♚ ");
}

std::ostream&
operator<<(std::ostream& os, King const& king)
{
    king.print(os);
    return os;
}
