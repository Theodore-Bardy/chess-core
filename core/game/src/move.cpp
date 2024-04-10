/**
 * @file  move.cpp
 * @brief Move pieces sources
 */

#include <iostream>
#include "move.hpp"

using namespace std;

Move::Move(Piece* _piece, int _xStart, int _yStart, int _xEnd, int _yEnd, int _flags)
    : piece(_piece)
    , xStart(_xStart)
    , yStart(_yStart)
    , xEnd(_xEnd)
    , yEnd(_yEnd)
    , flags(_flags)
{
}

void
Move::print(std::ostream& os) const
{
    os << *piece;

    /* Take flag */
    if (MOVE_FLAG_EAT == (flags & MOVE_FLAG_EAT))
    {
        os << "x";
    }

    /* Colomn */
    switch (xEnd)
    {
        case SQUARE_X_A:
            os << "a";
            break;
        case SQUARE_X_B:
            os << "b";
            break;
        case SQUARE_X_C:
            os << "c";
            break;
        case SQUARE_X_D:
            os << "d";
            break;
        case SQUARE_X_E:
            os << "e";
            break;
        case SQUARE_X_F:
            os << "f";
            break;
        case SQUARE_X_G:
            os << "g";
            break;
        case SQUARE_X_H:
            os << "h";
            break;
        default:
            /* Should not be here */
            break;
    }

    /* Row */
    switch (yEnd)
    {
        case SQUARE_Y_1:
            os << "1";
            break;
        case SQUARE_Y_2:
            os << "2";
            break;
        case SQUARE_Y_3:
            os << "3";
            break;
        case SQUARE_Y_4:
            os << "4";
            break;
        case SQUARE_Y_5:
            os << "5";
            break;
        case SQUARE_Y_6:
            os << "6";
            break;
        case SQUARE_Y_7:
            os << "7";
            break;
        case SQUARE_Y_8:
            os << "8";
            break;
        default:
            /* Should not be here */
            break;
    }
}

std::ostream&
operator<<(std::ostream& os, Move const& move)
{
    move.print(os);
    return os;
}
