/**
 * @file  bishop.cpp
 * @brief Bishop sources
 */

#include <assert.h>
#include <math.h>
#include "bishop.hpp"
#include "move.hpp"

using namespace std;

Bishop::Bishop(bool _color, int _x, int _y)
    : Piece(_color, _x, _y)
{
}

bool
Bishop::checkMove(int _x, int _y, int& flags, Square* board[8U][8U]) const
{
    /* Check parameter */
    assert(nullptr != board);

    bool xReturn = false;

    /* Check desired position exists and it is not the current position */
    if ((_x > 7) || (_x < 0) || (_y > 7) || (_y < 0) || ((_x == x) && (_y == y)))
    {
        return false;
    }

    /* The bishop moves along a diagonal, check the desired postion is reachable */
    if (sqrt(pow(x - _x, 2)) == sqrt(pow(y - _y, 2)))
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
Bishop::move(int _x, int _y, int& flags, Square* board[8U][8U])
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
Bishop::print(void) const
{
    return (color ? " ♗ " : " ♝ ");
}

std::ostream&
operator<<(std::ostream& os, Bishop const& bishop)
{
    os << bishop.print();
    return os;
}
