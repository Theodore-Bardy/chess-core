/**
 * @file  square.cpp
 * @brief Square sources
 */

#include "square.hpp"

using namespace std;

Square::Square(int _x, int _y)
    : color(SquarePieceColor::NoPiece)
    , value(SquarePieceValue::Empty)
    , x(_x)
    , y(_y)
{
}

string
Square::print(void) const
{
    string toReturn = "";

    switch (value)
    {
        case SquarePieceValue::Empty:
            toReturn += " . ";
            break;

        case SquarePieceValue::KingValue:
            if (color == SquarePieceColor::White)
            {
                toReturn += " ♔ ";
            }
            else
            {
                toReturn += " ♚ ";
            }
            break;

        case SquarePieceValue::QueenValue:
            if (color == SquarePieceColor::White)
            {
                toReturn += " ♕ ";
            }
            else
            {
                toReturn += " ♛ ";
            }
            break;

        case SquarePieceValue::BishopValue:
            if (color == SquarePieceColor::White)
            {
                toReturn += " ♗ ";
            }
            else
            {
                toReturn += " ♝ ";
            }
            break;

        case SquarePieceValue::KnightValue:
            if (color == SquarePieceColor::White)
            {
                toReturn += " ♘ ";
            }
            else
            {
                toReturn += " ♞ ";
            }
            break;

        case SquarePieceValue::RookValue:
            if (color == SquarePieceColor::White)
            {
                toReturn += " ♖ ";
            }
            else
            {
                toReturn += " ♜ ";
            }
            break;

        case SquarePieceValue::PawnValue:
            if (color == SquarePieceColor::White)
            {
                toReturn += " ♙ ";
            }
            else
            {
                toReturn += " ♟ ";
            }
            break;

        default:
            /* Should never be here */
            break;
    }

    return toReturn;
}

std::ostream&
operator<<(std::ostream& os, Square const& square)
{
    os << square.print();
    return os;
}
