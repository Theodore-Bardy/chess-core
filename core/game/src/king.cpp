/**
 * @file  king.cpp
 * @brief King sources
 */

#include "king.hpp"
#include "move.hpp"

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
    if (!hasMoved && !isCheck)
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

        hasMoved = true;
        return true;
    }

    return false;
}

bool
King::isAbleToMove(int _x, int _y, int flags, Square* board[8U][8U]) const
{
    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The king can castle if it hasn't move and isn't in check state */
    if (((MOVE_FLAG_KING_CASTLE == (flags & MOVE_FLAG_KING_CASTLE)) || (MOVE_FLAG_QUEEN_CASTLE == (flags & MOVE_FLAG_QUEEN_CASTLE))) && (_y == y))
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
    if (this->isAbleToMove(_x, _y, flags))
    {
        x        = _x;
        y        = _y;
        hasMoved = true;
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
    os << (color ? " ♔ " : " ♚ ");
}

std::ostream&
operator<<(std::ostream& os, King const& king)
{
    king.print(os);
    return os;
}
