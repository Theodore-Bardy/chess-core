/**
 * @file  board.hpp
 * @brief board header
 */

#ifndef _BOARD_HPP_
#define _BOARD_HPP_

class Piece;
#include "piece.hpp"
class Square;
#include "square.hpp"
#include <iostream>
#include <vector>

#define BOARD_SIZE_MAX (8)

class Board
{
protected:
    bool                isInitialize;
    Square*             board[BOARD_SIZE_MAX][BOARD_SIZE_MAX];
    std::vector<Piece*> white_pieces;
    std::vector<Piece*> balck_pieces;

public:
    /**
     * @brief Construct a new board
     */
    Board();

    /**
     * @brief Destroy the board
     */
    ~Board();

    /**
     * @brief Set the board with the default configuration
     */
    void startUp();

    /**
     * @brief Get the square at the given position on the board
     * @param[in] _x X square position
     * @param[in] _y Y square position
     * @return The square
     */
    Square* getSquare(int _x, int _y);

    /**
     * @brief Print board
     * @param[in, out] os Output stream
     */
    void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for board class
     * @param[in, out] os Output stream
     * @param[in] board Board object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Board const& board);
};

#endif /* _BOARD_HPP_ */
