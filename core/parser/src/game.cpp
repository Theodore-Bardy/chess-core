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
Game::addMove(int xStart, int yStart, int xEnd, int yEnd, int flags)
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
    moves.push_back(new Move(p, xStart, yStart, xEnd, yEnd));
    moveIndex++;

    return true;
}

bool
Game::addMoveFromPieceDesc(SquareValue pieceType, SquarePieceColor pieceColor, int xEnd, int yEnd, int flags)
{
    vector<Piece*> pieces;

    /* Select piece */
    if (gameBoard.lookForPiecesAbleToMoveAt(pieces, pieceType, pieceColor, xEnd, yEnd, flags))
    {
        /* Add move into moves vector */
        for (auto p : pieces)
        {
            return this->addMove(p->getX(), p->getY(), xEnd, yEnd, flags);
        }
    }

    return false;
}

void
Game::print(std::ostream& os) const
{
    int whiteMove = 1;
    int blackMove = 1;

    os << "[" << moveIndex << "]";
    for (auto m : moves)
    {
        if (whiteMove == blackMove)
        {
            os << " " << whiteMove << ". " << *m;
            whiteMove++;
        }
        else
        {
            os << " " << *m;
            blackMove++;
        }
    }

    os << "\n\n" << gameBoard;
}

std::ostream&
operator<<(std::ostream& os, Game const& game)
{
    game.print(os);
    return os;
}
