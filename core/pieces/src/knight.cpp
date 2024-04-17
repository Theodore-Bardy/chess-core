/**
 * @file  knight.cpp
 * @brief Knight sources
 */

#include <assert.h>
#include "knight.hpp"
#include "move.hpp"

using namespace std;

Knight::Knight(bool _color, int _x, int _y)
    : Piece(_color, _x, _y)
{
}

bool
Knight::checkMove(int _x, int _y, int& flags, Square* board[8U][8U]) const
{
    /* Check parameter */
    assert(nullptr != board);

    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* Check the desired postion is reachable */
    if (((_x == x + 2) && (_y == y + 1)) || ((_x == x + 2) && (_y == y - 1)) || ((_x == x - 2) && (_y == y + 1)) || ((_x == x - 2) && (_y == y - 1))
        || ((_x == x + 1) && (_y == y + 2)) || ((_x == x + 1) && (_y == y - 2)) || ((_x == x - 1) && (_y == y + 2)) || ((_x == x - 1) && (_y == y - 2)))
    {
        xReturn = true;
    }

    /* Check desired position isn't a piece of the same color */
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

    return xReturn;
}

bool
Knight::move(int _x, int _y, int& flags, Square* board[8U][8U])
{
    /* Check the king is able to move to the desired position */
    if (this->checkMove(_x, _y, flags, board))
    {
        x = _x;
        y = _y;
        return true;
    }

    return false;
}

string
Knight::print(void) const
{
    return (color ? " ♘ " : " ♞ ");
}

std::ostream&
operator<<(std::ostream& os, Knight const& knight)
{
    os << knight.print();
    return os;
}