/**
 * @file  pawn.cpp
 * @brief Pawn sources
 */

#include "pawn.hpp"
#include "move.hpp"

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
Pawn::isAbleToMove(int _x, int _y, int flags, Square* board[8U][8U]) const
{
    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* If the pawn is eating it must move 1 square right or left */
    if ((MOVE_FLAG_TAKE == (flags & MOVE_FLAG_TAKE)) && ((_x == x + 1) || (_x == x - 1)))
    {
        /* Check the desired postion is reachable */
        if (color)
        {
            /* White pawn must increase Y by one square */
            if (_y == y + 1)
            {
                xReturn = true;
            }
        }
        else
        {
            /* Black pawn must decrease Y by one square */
            if (_y == y - 1)
            {
                xReturn = true;
            }
        }
    }
    /* If the pawn isn't eating it must stay on his column */
    else if ((0 == (flags & MOVE_FLAG_TAKE)) && (_x == x))
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
Pawn::move(int _x, int _y, int flags)
{
    /* Check the king is able to move to the desired position */
    if (this->isAbleToMove(_x, _y, flags))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

SquarePieceValue
Pawn::getValue(void) const
{
    return PawnValue;
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
