/**
 * @file  game.cpp
 * @brief Chess game sources
 */

#include <iostream>
#include "game.hpp"

using namespace std;

Game::Game()
    : index(0)
{
}

void
Game::addMove(Move const& new_move)
{
    gameDescription.push_back(new_move);
}

Move&
Game::firstMove(void)
{
    index = 0;
    return gameDescription.at(index);
}

Move&
Game::previousMove(void)
{
    /* Decrement index except if it already the first move */
    index -= (index == 0) ? 0 : 1;
    return gameDescription.at(index);
}

Move&
Game::nextMove(void)
{
    index += (index == gameDescription.size() - 1U) ? 0 : 1;;
    return gameDescription.at(index);
}

Move&
Game::lastMove(void)
{
    index = gameDescription.size() - 1U;
    return gameDescription.at(index);
}

void
Game::print(std::ostream& os) const
{
    int whiteMove = 1;
    int blackMove = 1;

    os << "[" << index << "]";
    for (auto it : gameDescription)
    {
        if (whiteMove == blackMove)
        {
            os << " " << whiteMove << ". " << it;
            whiteMove++;
        }
        else
        {
            os << " " << blackMove << ".. " << it;
            blackMove++;
        }
    }
}

std::ostream&
operator<<(std::ostream& os, Game const& game)
{
    game.print(os);
    return os;
}
