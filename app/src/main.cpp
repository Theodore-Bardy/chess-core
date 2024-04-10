/**
 * @file  main.cpp
 * @brief main file
 */

#include "main.hpp"
#include <iostream>

#include "game.hpp"
#include "parser.hpp"

using namespace std;

/**
 * @brief Convert move string into [X,Y] position
 */
static bool computeMove(string const& move, int& x, int& y);

int
main(int argc, char const* argv[])
{
    Game gameBoard;

    cout << "   Chess core demo:" << endl;
    cout << gameBoard << endl;

    Piece* p = nullptr;
    string move;
    int    xStart;
    int    yStart;
    int    xEnd;
    int    yEnd;

    while (true)
    {
        cout << "Please select a piece: ";
        cin >> move;
        if (computeMove(move, xStart, yStart))
        {
            cout << "Please select a square to move the piece: ";
            cin >> move;
            if (computeMove(move, xEnd, yEnd))
            {
                if (gameBoard.addMove(xStart, yStart, xEnd, yEnd, 0))
                {
                    cout << gameBoard << endl;
                }
                else
                {
                    cout << "[Error] Unable to move the piece" << endl;
                }
            }
            else
            {
                cout << "[Error] Wrong input" << endl;
            }
        }
        else
        {
            cout << "[Error] Wrong input" << endl;
        }
    }

    return 0;
}

static bool
computeMove(string const& move, int& x, int& y)
{
    switch (move.at(0))
    {
        case 'a':
            x = SQUARE_X_A;
            break;
        case 'b':
            x = SQUARE_X_B;
            break;
        case 'c':
            x = SQUARE_X_C;
            break;
        case 'd':
            x = SQUARE_X_D;
            break;
        case 'e':
            x = SQUARE_X_E;
            break;
        case 'f':
            x = SQUARE_X_F;
            break;
        case 'g':
            x = SQUARE_X_G;
            break;
        case 'h':
            x = SQUARE_X_H;
            break;
        default:
            return false;
    }

    switch (move.at(1))
    {
        case '1':
            y = SQUARE_Y_1;
            break;
        case '2':
            y = SQUARE_Y_2;
            break;
        case '3':
            y = SQUARE_Y_3;
            break;
        case '4':
            y = SQUARE_Y_4;
            break;
        case '5':
            y = SQUARE_Y_5;
            break;
        case '6':
            y = SQUARE_Y_6;
            break;
        case '7':
            y = SQUARE_Y_7;
            break;
        case '8':
            y = SQUARE_Y_8;
            break;
        default:
            return false;
    }

    return true;
}
