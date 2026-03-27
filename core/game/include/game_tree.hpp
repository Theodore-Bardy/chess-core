/**
 * @file  game_tree.hpp
 * @brief Game tree for storing moves with variations (PGN branches)
 */

#ifndef _GAME_TREE_HPP_
#define _GAME_TREE_HPP_

#include <string>
#include <vector>

#include "board.hpp"
#include "move.hpp"

/**
 * @brief One node in the game tree.
 *        The root node has no move (piece == nullptr) and holds the starting position.
 *        Every other node holds the move that leads to it and the resulting board state.
 */
struct GameNode
{
    Move               move;     /**< Move applied to reach this position (piece == nullptr for root) */
    Board              board;    /**< Board state AFTER this move */
    std::string        comment;  /**< Optional annotation */
    GameNode*          parent;   /**< Parent node (nullptr for root) */
    std::vector<GameNode*> children; /**< children[0] = main line; children[1..] = variations */

    /**
     * @brief Construct a game node
     */
    GameNode(Move const& _move, Board const& _board, GameNode* _parent);

    /**
     * @brief Destroy node and all its children recursively
     */
    ~GameNode();
};

/**
 * @brief Tree of game positions supporting variations (PGN branches).
 *        The cursor tracks the current browsing position.
 */
class GameTree
{
private:
    GameNode*              root;           /**< Root: no move, starting position */
    GameNode*              cursor;         /**< Current position */
    std::vector<GameNode*> variationStack; /**< Saved cursors for nested variation parsing */

public:
    /**
     * @brief Construct a game tree with the given starting board
     */
    explicit GameTree(Board const& startingBoard);

    /**
     * @brief Destroy the tree (recursively deletes all nodes via root)
     */
    ~GameTree();

    /**
     * @brief Add a move from the current cursor position as the next step.
     *        Creates a new child node and advances the cursor to it.
     * @param move   The move that was played
     * @param board  The board state after the move
     */
    void addMove(Move const& move, Board const& board);

    /**
     * @brief Add a move as a variation at the current cursor position without advancing.
     *        Used by the PGN parser for nested parenthesised variations.
     * @param move   The alternative move
     * @param board  The board state after the alternative move
     */
    void addVariation(Move const& move, Board const& board);

    /**
     * @brief Save the current cursor and step back one ply.
     *        Called by the PGN parser when a '(' is encountered.
     */
    void startVariation();

    /**
     * @brief Restore the cursor saved by the matching startVariation().
     *        Called by the PGN parser when a ')' is encountered.
     */
    void endVariation();

    /**
     * @brief Move the cursor backward to the parent node.
     * @return true if successful (was not already at root), false otherwise
     */
    bool back();

    /**
     * @brief Move the cursor forward to a child node.
     * @param variation  Index of the child (0 = main line)
     * @return true if successful, false if no such child
     */
    bool forward(int variation = 0);

    /**
     * @brief Returns the current cursor node (read-only)
     */
    GameNode const* getCursor() const { return cursor; }

    /**
     * @brief Returns the root node (read-only)
     */
    GameNode const* getRoot() const { return root; }

    /**
     * @brief Returns whether the cursor can go backward
     */
    bool canGoBack() const { return cursor->parent != nullptr; }

    /**
     * @brief Returns whether the cursor can go forward on the given variation index
     */
    bool canGoForward(int variation = 0) const
    {
        return (variation >= 0) && (variation < (int)cursor->children.size());
    }

    /**
     * @brief Returns the number of half-moves from root to current cursor (depth)
     */
    int getDepth() const;

    /**
     * @brief Set the comment on the current cursor node
     */
    void setCursorComment(std::string const& comment) { cursor->comment = comment; }
};

#endif /* _GAME_TREE_HPP_ */
