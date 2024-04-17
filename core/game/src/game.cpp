/**
 * @file  game.cpp
 * @brief Chess game sources
 */

#include <iostream>
#include "game.hpp"

using namespace std;

Game::Game()
    : moveIndex(0)
{
    gameBoard.startUp();
}

Game::~Game()
{
    /* Release moves vector */
    for (Move* m : moves)
    {
        delete m;
    }
}

bool
Game::addMove(int xStart, int yStart, int xEnd, int yEnd, int flags, char extraFlag)
{
    Piece* p = nullptr;

    /* Select piece */
    if (!gameBoard.selectPiece(&p, xStart, yStart))
    {
        return false;
    }

    /* Move piece */
    if (!gameBoard.movePiece(p, xEnd, yEnd, flags))
    {
        return false;
    }

    /* Add move into moves vector */
    moves.push_back(new Move(p, xStart, yStart, xEnd, yEnd, flags, extraFlag));
    moveIndex++;

    return true;
}

bool
Game::checkMove(SquarePieceValue pieceType, SquarePieceColor pieceColor, int xEnd, int yEnd, int flags, char extraFlag)
{
    Piece* p = nullptr;

    /* Select piece */
    if (gameBoard.searchPiece(&p, pieceType, pieceColor, xEnd, yEnd, flags, extraFlag))
    {
        /* Add move into moves vector */
        return this->addMove(p->getX(), p->getY(), xEnd, yEnd, flags, extraFlag);
    }

    return false;
}

string
Game::print(void) const
{
    string toReturn = "";

    int whiteMove = 1;
    int blackMove = 1;

    toReturn += "[" + to_string(moveIndex) + "]";
    for (auto m : moves)
    {
        if (whiteMove == blackMove)
        {
            toReturn += " " + to_string(whiteMove) + ". " + m->print();
            whiteMove++;
        }
        else
        {
            toReturn += " " + m->print();
            blackMove++;
        }
    }

    toReturn += "\n\n" + gameBoard.print();

    return toReturn;
}

std::ostream&
operator<<(std::ostream& os, Game const& game)
{
    os << game.print();
    return os;
}
