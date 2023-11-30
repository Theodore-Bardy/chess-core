/**
 * @file  parser.cpp
 * @brief Parser sources
 */

#include "parser.hpp"
#include <iostream>
#include <optional>
#include <fstream>
#include <string>

using namespace std;

static optional<string> getMoveIntoLine(string const& line, int moveNumber, bool color);

bool
Parser::loadGame(string filePath, string fileName)
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

        do
        {
            whiteFound = false;

            /* Get white move */
            tempStr = getMoveIntoLine(gameAsStr, move, true);
            if (tempStr.has_value())
            {
                whiteFound = true;
                gameDescription.push_back(tempStr.value());
            }

            /* Get black move if white move have been found */
            if (whiteFound)
            {
                tempStr = getMoveIntoLine(gameAsStr, move++, false);
                if (tempStr.has_value())
                {
                    gameDescription.push_back(tempStr.value());
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
getMoveIntoLine(string const& line, int moveNumber, bool color)
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
