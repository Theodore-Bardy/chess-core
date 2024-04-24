/**
 * @file  pawn.cpp
 * @brief Pawn sources
 */

#include <assert.h>
#include "pawn.hpp"
#include "move.hpp"

Pawn::Pawn(bool _color, int _x)
    : Piece(_color, _x, 0)
    , hasMoved(false)
{
    if (_color)
    {
        y = PAWN_WHITE_DEFAULT_Y;
    }
    else
    {
        y = PAWN_BLACK_DEFAULT_Y;
    }
}

void
Pawn::promotion()
{
    // TODO
}

bool
Pawn::checkMove(int _x, int _y, int& flags, Square* board[8U][8U]) const
{
    /* Check parameter */
    assert(nullptr != board);

    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The pawn can move right or left only when it takes */
    if (((_x == x + 1) || (_x == x - 1)) && (this->checkFinalOnMove(_x, _y, board) == 1))
    {
        /* White (black) pawn must increase (decrease) Y of 1 on take move */
        if ((color && (_y == y + 1)) || (!color && (_y == y - 1)))
        {
            flags |= MOVE_FLAG_TAKE;
            xReturn = true;
        }
    }
    /* Otherwise the pawn must stay on his column */
    else if (_x == x)
    {
        /* Check the desired postion is reachable */
        if (color)
        {
            /* White pawn must increase Y at each move */
            if ((_y > y) && ((_y == y + 1) || (!hasMoved && (_y == y + 2))))
            {
                xReturn = true;
            }
        }
        else
        {
            /* Black pawn must decrease Y at each move */
            if ((_y < y) && ((_y == y - 1) || (!hasMoved && (_y == y - 2))))
            {
                xReturn = true;
            }
        }
    }

    /* Check if there is piece between current and desired position */
    if (xReturn)
    {
        xReturn = this->checkWayOnMove(_x, _y, board);
        if (xReturn)
        {
            /* Check for piece of the same color */
            if (this->checkFinalOnMove(_x, _y, board) == -1)
            {
                xReturn = false;
            }
        }
    }

    /* Check if pawn reach the end of the board */
    if (xReturn && ((color && (_y == SQUARE_Y_8)) || (!color && (_y == SQUARE_Y_1))))
    {
        flags |= MOVE_FLAG_PROMOTION;
    }

    return xReturn;
}

bool
Pawn::move(int _x, int _y, int& flags, Square* board[8U][8U])
{
    /* Check the king is able to move to the desired position */
    if (this->checkMove(_x, _y, flags, board))
    {
        x        = _x;
        y        = _y;
        hasMoved = true;
        return true;
    }

    return false;
}

void
Pawn::print(std::ostream& os) const
{
    os << (color ? " ♙ " : " ♟ ");
}

std::ostream&
operator<<(std::ostream& os, Pawn const& pawn)
{
    pawn.print(os);
    return os;
}
