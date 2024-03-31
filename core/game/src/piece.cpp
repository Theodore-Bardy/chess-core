/**
 * @file  piece.cpp
 * @brief Piece sources
 */

#include "piece.hpp"

Piece::Piece(bool _color, int _x, int _y)
    : isAlive(true)
    , color(_color)
    , y(_y)
    , x(_x)
{
}

Piece::Piece(bool _isAlive, bool _color, int _x, int _y)
    : isAlive(_isAlive)
    , color(_color)
    , y(_y)
    , x(_x)
{
}

Piece::Piece(Piece const& piece)
    : isAlive(piece.isAlive)
    , color(piece.color)
    , y(piece.y)
    , x(piece.x)
{
}

Piece::~Piece()
{
}

void
Piece::eat(Piece& piece_to_eat)
{
    piece_to_eat.isAlive = false;
}

std::ostream&
operator<<(std::ostream& os, Piece const& piece)
{
    piece.print(os);
    return os;
}
