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

Piece::Piece(bool _alive, bool _color, int _x, int _y)
    : alive(_alive)
    , color(_color)
    , y(_y)
    , x(_x)
{
}

Piece::~Piece()
{
}

bool
Piece::checkWayOnMove(int _x, int _y, Square* board[8U][8U]) const
{
    assert(nullptr != board);

    int xDir   = ((x - _x) == 0) ? 0 : ((x - _x) > 0) ? 1 : -1;
    int yDir   = ((y - _y) == 0) ? 0 : ((y - _y) > 0) ? 1 : -1;
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

bool
Piece::checkFinalOnMove(int _x, int _y, Square* board[8U][8U]) const
{
    assert(nullptr != board);

    return (board[_x][_y]->getColor() != board[x][y]->getColor());
}

void
Piece::eat(Piece& piece_to_eat)
{
    piece_to_eat.alive = false;
}

std::ostream&
operator<<(std::ostream& os, Piece const& piece)
{
    piece.print(os);
    return os;
}
