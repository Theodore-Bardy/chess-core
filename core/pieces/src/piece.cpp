/**
 * @file  piece.cpp
 * @brief Piece sources
 */

#include <assert.h>
#include "piece.hpp"

Piece::Piece(bool _color, int _x, int _y)
    : alive(true)
    , color(_color)
    , y(_y)
    , x(_x)
{
}

bool
Piece::checkWayOnMove(int _x, int _y, Square* board[8U][8U]) const
{
    assert(nullptr != board);

    int xDir   = ((_x - x) == 0) ? 0 : ((_x - x) > 0) ? 1 : -1;
    int yDir   = ((_y - y) == 0) ? 0 : ((_y - y) > 0) ? 1 : -1;
    int xCheck = x + xDir;
    int yCheck = y + yDir;

    /* Check the way between current and desired position excluded is empty */
    while ((xCheck != _x) && (yCheck != _y))
    {
        if (board[xCheck][yCheck]->getValue() != SquarePieceValue::Empty)
        {
            return false;
        }
        xCheck += xDir;
        yCheck += yDir;
    }

    return true;
}

int
Piece::checkFinalOnMove(int _x, int _y, Square* board[8U][8U]) const
{
    assert(nullptr != board);

    /* Piece with the same color*/
    if (board[_x][_y]->getColor() == board[x][y]->getColor())
    {
        return -1;
    }
    /* No piece */
    else if (board[_x][_y]->getColor() == SquarePieceColor::NoPiece)
    {
        return 0;
    }

    /* Piece with the opposite color */
    return 1;
}

void
Piece::take(Piece& piece_to_take)
{
    piece_to_take.alive = false;
}

std::ostream&
operator<<(std::ostream& os, Piece const& piece)
{
    piece.print(os);
    return os;
}
