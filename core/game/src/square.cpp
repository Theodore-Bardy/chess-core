/**
 * @file  square.cpp
 * @brief Square sources
 */

#include "square.hpp"

Square::Square(int _x, int _y)
    : color(SquarePieceColor::NoPiece)
    , value(SquarePieceValue::Empty)
    , x(_x)
    , y(_y)
{
}

void
Square::print(std::ostream& os) const
{
    switch (value)
    {
        case SquarePieceValue::Empty:
            os << " . ";
            break;

        case SquarePieceValue::KingValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♔ ";
            }
            else
            {
                os << " ♚ ";
            }
            break;

        case SquarePieceValue::QueenValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♕ ";
            }
            else
            {
                os << " ♛ ";
            }
            break;

        case SquarePieceValue::BishopValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♗ ";
            }
            else
            {
                os << " ♝ ";
            }
            break;

        case SquarePieceValue::KnightValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♘ ";
            }
            else
            {
                os << " ♞ ";
            }
            break;

        case SquarePieceValue::RookValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♖ ";
            }
            else
            {
                os << " ♜ ";
            }
            break;

        case SquarePieceValue::PawnValue:
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
