/**
 * @file  square.cpp
 * @brief Square sources
 */

#include "square.hpp"

Square::Square(int _x, int _y)
    : color(SquarePieceColor::NoPiece)
    , value(SquareValue::Empty)
    , x(_x)
    , y(_y)
{
}

void
Square::print(std::ostream& os) const
{
    switch (value)
    {
        case Empty:
            os << " . ";
            break;

        case KingValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♔ ";
            }
            else
            {
                os << " ♚ ";
            }
            break;

        case QueenValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♕ ";
            }
            else
            {
                os << " ♛ ";
            }
            break;

        case BishopValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♗ ";
            }
            else
            {
                os << " ♝ ";
            }
            break;

        case KnightValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♘ ";
            }
            else
            {
                os << " ♞ ";
            }
            break;

        case RookValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♖ ";
            }
            else
            {
                os << " ♜ ";
            }
            break;

        case PawnValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♙ ";
            }
            else
            {
                os << " ♟ ";
            }
            break;

        default:
            /* Should never be here */
            break;
    }
}

std::ostream&
operator<<(std::ostream& os, Square const& square)
{
    square.print(os);
    return os;
}
