/**
 * @file  king.cpp
 * @brief King sources
 */

#include <assert.h>
#include "king.hpp"
#include "move.hpp"

using namespace std;

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
King::castle(bool side, Rook* rook)
{
    if (!hasMoved && !isCheck)
    {
        if (rook->castle())
        {
            x = (side) ? KING_KING_CASTLE_X : KING_QUEEN_CASTLE_X;
        }
        hasMoved = true;
        return true;
    }

    return false;
}

bool
King::checkMove(int _x, int _y, int& flags, Square* board[8U][8U]) const
{
    /* Check parameter */
    assert(nullptr != board);

    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* Check for king castle move */
    if (!hasMoved && !isCheck && (_y == y) && (_x == KING_KING_CASTLE_X) && (this->checkFinalOnMove(_x, _y, board) == 0))
    {
        flags |= MOVE_FLAG_KING_CASTLE;
        xReturn = this->checkWayOnMove(_x, _y, board);
    }
    /* Check for queen castle move */
    else if (!hasMoved && !isCheck && (_y == y) && (_x == KING_QUEEN_CASTLE_X) && (this->checkFinalOnMove(_x, _y, board) == 0))
    {
        flags |= MOVE_FLAG_QUEEN_CASTLE;
        xReturn = this->checkWayOnMove(_x, _y, board);
    }
    /* The king moves one square in any direction, check the desired postion is reachable */
    else if ((_x <= (x + 1)) && (_x >= (x - 1)) && (_y <= (y + 1)) && (_y >= (y - 1)))
    {
        xReturn = true;

        /* Check for piece of the opposite color */
        if (this->checkFinalOnMove(_x, _y, board) == 1)
        {
            flags |= MOVE_FLAG_TAKE;
        }
    }

    /* Check for piece of the same color */
    if (xReturn && this->checkFinalOnMove(_x, _y, board) == -1)
    {
        xReturn = false;
    }

    return xReturn;
}

bool
King::move(int _x, int _y, int& flags, Square* board[8U][8U])
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

string
King::print(void) const
{
    return (color ? " ♔ " : " ♚ ");
}

std::ostream&
operator<<(std::ostream& os, King const& king)
{
    os << king.print();
    return os;
}
