/**
 * @file  rook.cpp
 * @brief Rook sources
 */

#include <assert.h>
#include "rook.hpp"
#include "move.hpp"

Rook::Rook(bool _color, int _x, int _y)
    : Piece(_color, _x, _y)
    , hasMoved(false)
{
    /* If rook is create in h1 or h8 it's a king side rook */
    if (((ROOK_2_WHITE_DEFAULT_X == _x) && (ROOK_2_WHITE_DEFAULT_Y)) || ((ROOK_2_BLACK_DEFAULT_X == _x) && (ROOK_2_BLACK_DEFAULT_Y)))
    {
        isKingSide = true;
    }
    else
    {
        isKingSide = false;
    }
}

bool
Rook::castle(void)
{
    /* King side castle */
    if (!hasMoved && isKingSide && (x == ROOK_2_WHITE_DEFAULT_X))
    {
        x        = ROOK_KING_CASTLE_X;
        hasMoved = true;
        return true;
    }
    /* Queen side castle */
    else if (!hasMoved && !isKingSide && (x == ROOK_1_WHITE_DEFAULT_X))
    {
        x        = ROOK_QUEEN_CASTLE_X;
        hasMoved = true;
        return true;
    }

    return false;
}

bool
Rook::checkMove(int _x, int _y, int& flags, Square* board[8U][8U]) const
{
    /* Check parameter */
    assert(nullptr != board);

    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The rook moves along a rank, check the desired postion is reachable */
    else if ((_x == x) || (_y == y))
    {
        xReturn = true;
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
            /* Check for piece of the opposite color */
            else if (this->checkFinalOnMove(_x, _y, board) == 1)
            {
                flags |= MOVE_FLAG_TAKE;
            }
        }
    }

    return xReturn;
}

bool
Rook::move(int _x, int _y, int& flags, Square* board[8U][8U])
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
Rook::print(std::ostream& os) const
{
    os << (color ? " ♖ " : " ♜ ");
}

std::ostream&
operator<<(std::ostream& os, Rook const& rook)
{
    rook.print(os);
    return os;
}
