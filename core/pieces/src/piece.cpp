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
Piece::take(Piece& piece_to_take, int& flags)
{
    /* Check piece_to_take status */
    if (!piece_to_take.alive || piece_to_take.getColor() == color)
    {
        return false;
    }

    /* Move piece to the piece_to_take position */
    if (this->move(piece_to_take.getX(), piece_to_take.getY(), flags))
    {
        piece_to_take.alive = false;
        return true;
    }

    return false;
}

vector<pair<int, int>>
Piece::getAllMoves(void) const
{
    vector<pair<int, int>> moves;
    int                    flags = 0;

    /* Check all all square of the board */
    for (int _x = 0; _x < 8U; ++_x)
    {
        for (int _y = 0; _y < 8U; ++_y)
        {
            /* If the piece is able to move add the position to the vector */
            if (this->checkMove(_x, _y, flags))
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
