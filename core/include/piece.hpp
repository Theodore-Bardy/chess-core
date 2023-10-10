/**
 * @file      piece.hpp
 * @brief     Piece header
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include <iostream>

class piece
{
#define PIECE_WHITE (true)
#define PIECE_BLACK (false)

protected:
    bool isAlive; /**< Alive state */
    bool color;   /**< Color true = withe, false = black */
    int  x;       /**< X position */
    int  y;       /**< Y position */

public:
    /**
     * @brief Forbid the default constructor
     */
    piece() = delete;

    /**
     * @brief Construct a new piece
     * @param[in] _color Color of the piece
     * @param[in] _x Default X position
     * @param[in] _y Default Y position
     */
    piece(bool _color, int _x, int _y);

    /**
     * @brief Construct a new piece
     * @param[in] _isAlive Alive state
     * @param[in] _color Color of the piece
     * @param[in] _x Default X position
     * @param[in] _y Default Y position
     */
    piece(bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the piece
     */
    ~piece();

    /**
     * @brief Eat a piece
     * @param[in, out] piece_to_eat Piece to eat
     */
    void eat(piece& piece_to_eat);

    /**
     * @brief Move a piece
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @return true if the piece has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y) = 0;

    /**
     * @brief Get the value of the piece
     * @return The value of the piece
     */
    virtual int getValue(void) const = 0;

    /**
     * @brief Print the piece in the standard output
     */
    virtual void print(std::ostream &os) const = 0;
};

#endif /* _PIECE_HPP_ */
