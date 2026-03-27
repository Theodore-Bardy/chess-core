/**
 * @file  game_tree.cpp
 * @brief Game tree sources
 */

#include "game_tree.hpp"

/* ── GameNode ─────────────────────────────────────────────── */

GameNode::GameNode(Move const& _move, Board const& _board, GameNode* _parent)
    : move(_move)
    , board(_board)
    , comment()
    , parent(_parent)
    , children()
{
}

GameNode::~GameNode()
{
    for (GameNode* child : children)
    {
        delete child;
    }
}

/* ── GameTree ─────────────────────────────────────────────── */

GameTree::GameTree(Board const& startingBoard)
{
    /* Root node: sentinel move with nullptr piece represents the starting position */
    Move sentinel(nullptr, -1, -1, -1, -1, 0);
    root   = new GameNode(sentinel, startingBoard, nullptr);
    cursor = root;
}

GameTree::~GameTree()
{
    delete root;
}

void
GameTree::addMove(Move const& move, Board const& board)
{
    GameNode* node = new GameNode(move, board, cursor);
    cursor->children.push_back(node);
    cursor = node;
}

void
GameTree::addVariation(Move const& move, Board const& board)
{
    GameNode* node = new GameNode(move, board, cursor);
    cursor->children.push_back(node);
    /* cursor does NOT advance — we stay at the current position */
}

void
GameTree::startVariation()
{
    /* Save current cursor so endVariation() can restore it */
    variationStack.push_back(cursor);
    /* Step back one ply to the position before the variation branch */
    if (cursor->parent != nullptr)
    {
        cursor = cursor->parent;
    }
}

void
GameTree::endVariation()
{
    if (!variationStack.empty())
    {
        cursor = variationStack.back();
        variationStack.pop_back();
    }
}

bool
GameTree::back()
{
    if (cursor->parent == nullptr)
    {
        return false;
    }
    cursor = cursor->parent;
    return true;
}

bool
GameTree::forward(int variation)
{
    if ((variation < 0) || (variation >= (int)cursor->children.size()))
    {
        return false;
    }
    cursor = cursor->children[variation];
    return true;
}

int
GameTree::getDepth() const
{
    int depth = 0;
    GameNode const* node = cursor;
    while (node->parent != nullptr)
    {
        depth++;
        node = node->parent;
    }
    return depth;
}
