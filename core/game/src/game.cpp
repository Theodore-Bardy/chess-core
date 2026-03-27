/**
 * @file  game.cpp
 * @brief Chess game sources
 */

#include <iostream>
#include "game.hpp"

using namespace std;

static Board makeStartingBoard()
{
    Board b;
    b.startUp();
    return b;
}

Game::Game()
    : tree(makeStartingBoard())
    , colorToMove(true)
    , fullMoveNumber(1)
{
    gameBoard.startUp();
}

Game::~Game()
{
}

bool
Game::addMove(int xStart, int yStart, int xEnd, int yEnd, int flags, char extraFlag,
              SquarePieceValue promotionPiece)
{
    Piece* p = nullptr;

    /* Select piece */
    if (!gameBoard.selectPiece(&p, xStart, yStart))
    {
        return false;
    }

    /* Reject moves that leave the own king in check */
    if (!gameBoard.isMoveLegal(p, xEnd, yEnd, flags))
    {
        return false;
    }

    /* Move piece on working board */
    if (!gameBoard.movePiece(p, xEnd, yEnd, flags, promotionPiece))
    {
        return false;
    }

    /* Store move and board snapshot in the game tree */
    Move m(p, xStart, yStart, xEnd, yEnd, flags, extraFlag, promotionPiece);
    tree.addMove(m, gameBoard);

    /* Advance clocks */
    if (!colorToMove)
    {
        fullMoveNumber++;
    }
    colorToMove = !colorToMove;

    return true;
}

bool
Game::checkMove(SquarePieceValue pieceType, SquarePieceColor pieceColor, int xEnd, int yEnd,
                int flags, char extraFlag, SquarePieceValue promotionPiece)
{
    Piece* p = nullptr;

    if (gameBoard.checkMove(&p, pieceType, pieceColor, xEnd, yEnd, flags, extraFlag))
    {
        return this->addMove(p->getX(), p->getY(), xEnd, yEnd, flags, extraFlag, promotionPiece);
    }

    return false;
}

bool
Game::forward(int variation)
{
    if (!tree.canGoForward(variation))
    {
        return false;
    }

    /* Advance tree cursor */
    tree.forward(variation);

    /* Update working board to match the new cursor's snapshot */
    gameBoard = tree.getCursor()->board;

    /* Update clocks */
    if (!colorToMove)
    {
        fullMoveNumber++;
    }
    colorToMove = !colorToMove;

    return true;
}

bool
Game::back()
{
    if (!tree.canGoBack())
    {
        return false;
    }

    /* Step back one ply */
    tree.back();

    /* Restore working board from the (now current) cursor's snapshot */
    gameBoard = tree.getCursor()->board;

    /* Undo clock advancement */
    colorToMove = !colorToMove;
    if (!colorToMove)
    {
        fullMoveNumber--;
    }

    return true;
}

void
Game::startVariation()
{
    tree.startVariation();
    gameBoard = tree.getCursor()->board;

    /* Undo one half-move's worth of clock state */
    colorToMove = !colorToMove;
    if (!colorToMove)
    {
        fullMoveNumber--;
    }
}

void
Game::endVariation()
{
    /* Restore cursor to saved position */
    GameNode const* savedCursor = tree.getCursor();
    tree.endVariation();
    GameNode const* restoredCursor = tree.getCursor();

    /* Only update clocks/board if the cursor actually changed */
    if (savedCursor != restoredCursor)
    {
        gameBoard = restoredCursor->board;

        /* Recompute colorToMove and fullMoveNumber from depth */
        int depth = tree.getDepth();
        colorToMove   = (depth % 2 == 0); /* even depth = white to move */
        fullMoveNumber = 1 + (depth / 2);
    }
}

std::string
Game::getFEN() const
{
    return gameBoard.getFEN(colorToMove, fullMoveNumber);
}

void
Game::print(std::ostream& os) const
{
    /* Walk the main line from root to end to print moves */
    int whiteMove = 1;
    int blackMove = 1;

    int depth = tree.getDepth();
    os << "[" << depth << "]";

    /* Collect main-line nodes from root to the deepest main-line node */
    std::vector<GameNode const*> mainLine;
    GameNode const* node = tree.getRoot();
    while (!node->children.empty())
    {
        node = node->children[0]; /* always follow main line */
        mainLine.push_back(node);
    }

    for (GameNode const* n : mainLine)
    {
        if (whiteMove == blackMove)
        {
            os << " " << whiteMove << ". " << n->move;
            whiteMove++;
        }
        else
        {
            os << " " << n->move;
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
