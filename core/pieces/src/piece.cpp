/**
 * @file  piece.cpp
 * @brief Piece sources
 */

#include <assert.h>
#include "piece.hpp"

using namespace std;

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
    while ((xCheck != _x) || (yCheck != _y))
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

vector<pair<int, int>>
Piece::getAllMoves(Square* board[8U][8U]) const
{
    assert(nullptr != board);

    vector<pair<int, int>> moves;
    int                    flags = 0;

    /* Check all all square of the board */
    for (int _x = 0; _x < 8U; ++_x)
    {
        for (int _y = 0; _y < 8U; ++_y)
        {
            /* If the piece can move add the position to the vector */
            if (this->checkMove(_x, _y, flags, board))
            {
                moves.push_back({ _x, _y });
            }
        }
    }

    return moves;
}

std::ostream&
operator<<(std::ostream& os, Piece const& piece)
{
    piece.print(os);
    return os;
}
