/**
 * @file  piece.cpp
 * @brief Piece sources
 */

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
