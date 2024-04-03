/**
 * @file  parser.cpp
 * @brief Parser sources
 */

#include "parser.hpp"
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

/**
 * @brief Get the Move String Into Line object
 * @param[in] line Game description line
 * @param[in] moveNumber look for this move number
 * @param[in] color Look for this color (true: white, false: black)
 * @return The string description of the move
 */
static optional<string> getMoveStringIntoLine(string const& line, int moveNumber, bool color);

/**
 * @brief Parse move informations from the move string
 * @param[in] move Move string
 * @param[out] pieceType Piece type
 * @param[out] xEnd X position
 * @param[out] yEnd Y position
 * @param[out] flags Move flags
 * @return true if the move string have been parsed, false otherwise
 */
static bool getMoveDescriptionFromString(string const& move, SquareValue& pieceType, int& xEnd, int& yEnd, int& flags);

/**
 * @brief Returns the X position depending on the given char
 * @param[in] c The char
 * @return The X position
 */
static int getXfromChar(char const& c);

/**
 * @brief Returns the Y position depending on the given char
 * @param[in] c The char
 * @return The Y position
 */
static int getYfromChar(char const& c);

bool
Parser::loadGame(Game& game, string filePath, string fileName)
{
    ifstream fileToRead;
    string   gameAsStr;
    string   line;
    bool     startFound = false;
    int      move       = 1;

    /* Open file */
    fileToRead.open(fileName);
    if (!fileToRead.is_open())
    {
        return false;
    }

    /* Read file */
    while (fileToRead)
    {
        /* Look for the first move */
        if (!startFound)
        {
            /* Search for first move "1. " */
            while (getline(fileToRead, line) && !line.find("1. "))
            {
                startFound = true;
                gameAsStr += line + " ";
                break;
            }
        }
        else
        {
            /* Concatanate all lines after the first move to make easier the parsing */
            getline(fileToRead, line);
            gameAsStr += line + " ";
        }
    }

    /* Check the first move have been found */
    if (startFound)
    {
        optional<string> tempStr;
        bool             whiteFound;
        SquareValue      pieceType;
        int              xEnd;
        int              yEnd;
        int              flags;

        do
        {
            whiteFound = false;

            /* Get white move */
            tempStr = getMoveStringIntoLine(gameAsStr, move, true);
            if (tempStr.has_value() && getMoveDescriptionFromString(tempStr.value(), pieceType, xEnd, yEnd, flags))
            {
                whiteFound = true;
                game.addMoveFromPieceDesc(pieceType, White, xEnd, yEnd, flags);
            }

            /* Get black move if white move have been found */
            if (whiteFound)
            {
                tempStr = getMoveStringIntoLine(gameAsStr, move++, false);
                if (tempStr.has_value() && getMoveDescriptionFromString(tempStr.value(), pieceType, xEnd, yEnd, flags))
                {
                    game.addMoveFromPieceDesc(pieceType, Black, xEnd, yEnd, flags);
                }
            }
        } while (tempStr.has_value());
    }

    return true;
}

bool
Parser::saveGame(string filePath, string fileName)
{
    return true;
}

static optional<string>
getMoveStringIntoLine(string const& line, int moveNumber, bool color)
{
    string query;
    size_t found;
    size_t start;
    size_t end;

    /* Look for a white move */
    if (color)
    {
        query = to_string(moveNumber) + ". ";
    }
    /* Look for a black move */
    else
    {
        query = to_string(moveNumber) + "... ";
    }

    found = line.find(query);
    if (found != string::npos)
    {
        start = found + query.length();
        end   = line.find(" ", start);

        if (found != string::npos)
        {
            string ret = line.substr(start, end - start);
            return ret;
        }
    }

    return nullopt;
}

static bool
getMoveDescriptionFromString(string const& move, SquareValue& pieceType, int& xEnd, int& yEnd, int& flags)
{
    /* Reset flags */
    flags = 0;

    /* Special moves */
    // TODO - castle - promotion …

    /* Pawn moves */
    if (islower(move.at(0)))
    {
        pieceType = PawnValue;

        /* Check if the pawn is eating */
        if (move.at(1) == 'x')
        {
            flags |= MOVE_FLAG_EAT;
            xEnd = getXfromChar(move.at(2));
            yEnd = getYfromChar(move.at(3));
        }
        else
        {
            xEnd = getXfromChar(move.at(0));
            yEnd = getYfromChar(move.at(1));
        }
    }

    /* Other piece moves */
    // TODO

    return true;
}

static int
getXfromChar(char const& c)
{
    switch (c)
    {
        case 'a':
            return SQUARE_X_A;
        case 'b':
            return SQUARE_X_B;
        case 'c':
            return SQUARE_X_C;
        case 'd':
            return SQUARE_X_D;
        case 'e':
            return SQUARE_X_E;
        case 'f':
            return SQUARE_X_F;
        case 'g':
            return SQUARE_X_G;
        case 'h':
            return SQUARE_X_H;
        default:
            /* Should not be here */
            assert(false);
            return 0xFF;
    }
}

static int
getYfromChar(char const& c)
{
    switch (c)
    {
        case '1':
            return SQUARE_Y_1;
        case '2':
            return SQUARE_Y_2;
        case '3':
            return SQUARE_Y_3;
        case '4':
            return SQUARE_Y_4;
        case '5':
            return SQUARE_Y_5;
        case '6':
            return SQUARE_Y_6;
        case '7':
            return SQUARE_Y_7;
        case '8':
            return SQUARE_Y_8;
        default:
            /* Should not be here */
            assert(false);
            return 0xFF;
    }
}
