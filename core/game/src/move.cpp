/**
 * @file  move.cpp
 * @brief Move pieces sources
 */

#include <iostream>
#include "move.hpp"

using namespace std;

Move::Move(Piece* _piece, int _xStart, int _yStart, int _xEnd, int _yEnd, int _flags, char _extraFlag)
    : piece(_piece)
    , xStart(_xStart)
    , yStart(_yStart)
    , xEnd(_xEnd)
    , yEnd(_yEnd)
    , flags(_flags)
    , extraFlag(_extraFlag)
{
}

string
Move::print(void) const
{
    string toReturn = "";

    if (MOVE_FLAG_KING_CASTLE == (flags & MOVE_FLAG_KING_CASTLE))
    {
        toReturn += "O-O";
    }
    else if (MOVE_FLAG_KING_CASTLE == (flags & MOVE_FLAG_KING_CASTLE))
    {
        toReturn += "O-O-O";
    }
    else
    {
        toReturn += piece->print();

        /* Take flag */
        if (MOVE_FLAG_TAKE == (flags & MOVE_FLAG_TAKE))
        {
            toReturn += "x";
        }

        /* Colomn */
        switch (xEnd)
        {
            case SQUARE_X_A:
                toReturn += "a";
                break;
            case SQUARE_X_B:
                toReturn += "b";
                break;
            case SQUARE_X_C:
                toReturn += "c";
                break;
            case SQUARE_X_D:
                toReturn += "d";
                break;
            case SQUARE_X_E:
                toReturn += "e";
                break;
            case SQUARE_X_F:
                toReturn += "f";
                break;
            case SQUARE_X_G:
                toReturn += "g";
                break;
            case SQUARE_X_H:
                toReturn += "h";
                break;
            default:
                /* Should not be here */
                break;
        }

        /* Row */
        switch (yEnd)
        {
            case SQUARE_Y_1:
                toReturn += "1";
                break;
            case SQUARE_Y_2:
                toReturn += "2";
                break;
            case SQUARE_Y_3:
                toReturn += "3";
                break;
            case SQUARE_Y_4:
                toReturn += "4";
                break;
            case SQUARE_Y_5:
                toReturn += "5";
                break;
            case SQUARE_Y_6:
                toReturn += "6";
                break;
            case SQUARE_Y_7:
                toReturn += "7";
                break;
            case SQUARE_Y_8:
                toReturn += "8";
                break;
            default:
                /* Should not be here */
                break;
        }
    }

    return toReturn;
}

std::ostream&
operator<<(std::ostream& os, Move const& move)
{
    os << move.print();
    return os;
}
