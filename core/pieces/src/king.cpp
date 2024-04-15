/**
 * @file  king.cpp
 * @brief King sources
 */

#include "king.hpp"
#include "move.hpp"

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
King::castling(bool side)
{
    if (!hasMoved && !isCheck)
    {
        if (side)
        {
            x += 2;
        }
        else
        {
            x -= 2;
        }

        hasMoved = true;
        return true;
    }

    return false;
}

bool
King::checkMove(int _x, int _y, int flags, Square* board[8U][8U]) const
{
    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The king can castle if it hasn't move and isn't in check state */
    if ((MOVE_FLAG_KING_CASTLE == (flags & MOVE_FLAG_KING_CASTLE)) && (_y == y) && (_x == KING_KING_CASTLE_X))
    {
        xReturn = (!hasMoved && !isCheck);
    }
    else if ((MOVE_FLAG_QUEEN_CASTLE == (flags & MOVE_FLAG_QUEEN_CASTLE)) && (_y == y) && (_x == KING_QUEEN_CASTLE_X))
    {
        xReturn = (!hasMoved && !isCheck);
    }

    /* The king moves one square in any direction, check the desired postion is reachable */
    else if ((_x <= (x + 1)) && (_x >= (x - 1)) && (_y <= (y + 1)) && (_y >= (y - 1)))
    {
        xReturn = true;
    }

    /* Check if there is piece between current and desired position */
    if (xReturn && (board != nullptr))
    {
        xReturn = this->checkWayOnMove(_x, _y, board);
        if (xReturn)
        {
            xReturn = this->checkFinalOnMove(_x, _y, board);
        }
    }

    return xReturn;
}

bool
King::move(int _x, int _y, int flags)
{
    /* Check the king is able to move to the desired position */
    if (this->checkMove(_x, _y, flags))
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
