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
        case SquareValue::Empty:
            os << " . ";
            break;

        case SquareValue::KingValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♔ ";
            }
            else
            {
                os << " ♚ ";
            }
            break;

        case SquareValue::QueenValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♕ ";
            }
            else
            {
                os << " ♛ ";
            }
            break;

        case SquareValue::BishopValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♗ ";
            }
            else
            {
                os << " ♝ ";
            }
            break;

        case SquareValue::KnightValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♘ ";
            }
            else
            {
                os << " ♞ ";
            }
            break;

        case SquareValue::RookValue:
            if (color == SquarePieceColor::White)
            {
                os << " ♖ ";
            }
            else
            {
                os << " ♜ ";
            }
            break;

        case SquareValue::PawnValue:
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
