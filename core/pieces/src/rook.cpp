/**
 * @file  rook.cpp
 * @brief Rook sources
 */

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

Rook::Rook(bool _hasMoved, bool _isAlive, bool _color, int _x, int _y)
    : hasMoved(_hasMoved)
    , isKingSide(false)
    , Piece(_isAlive, _color, _x, _y)
{
}

Rook::~Rook()
{
}

bool
Rook::castling(bool side)
{
    /* King side castle */
    if (!hasMoved && side && (x == ROOK_2_WHITE_DEFAULT_X))
    {
        x        = ROOK_KING_CASTLE_X;
        hasMoved = true;
        return true;
    }
    /* Queen side castle */
    else if (!hasMoved && !side && (x == ROOK_1_WHITE_DEFAULT_X))
    {
        x        = ROOK_QUEEN_CASTLE_X;
        hasMoved = true;
        return true;
    }

    return false;
}

bool
Rook::isAbleToMove(int _x, int _y, int flags, Square* board[8U][8U]) const
{
    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The rook can castle if it hasn't move */
    if ((MOVE_FLAG_KING_CASTLE == (flags & MOVE_FLAG_KING_CASTLE)) && (_y == y) && (_x == ROOK_KING_CASTLE_X))
    {
        xReturn = (!hasMoved && isKingSide);
    }
    else if ((MOVE_FLAG_QUEEN_CASTLE == (flags & MOVE_FLAG_QUEEN_CASTLE)) && (_y == y) && (_x == ROOK_QUEEN_CASTLE_X))
    {
        xReturn = (!hasMoved && !isKingSide);
    }

    /* The rook moves along a rank, check the desired postion is reachable */
    else if ((_x == x) || (_y == y))
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
Rook::move(int _x, int _y, int flags)
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

SquarePieceValue
Rook::getValue(void) const
{
    return RookValue;
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
