/**
 * @file      queen.hpp
 * @brief     Queen header
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#ifndef _QUEEN_HPP_
#define _QUEEN_HPP_

#include "piece.hpp"

/**
 * @brief Queen default value
 */
#define QUEEN_VALUE           (9)
#define QUEEN_BLACK_DEFAULT_X (4)
#define QUEEN_BLACK_DEFAULT_Y (8)
#define QUEEN_WHITE_DEFAULT_X (4)
#define QUEEN_WHITE_DEFAULT_Y (1)

class Queen : public Piece
{
public:
    /**
     * @brief Forbid the default constructor
     */
    Queen() = delete;

    /**
     * @brief Construct a default queen
     */
    Queen(bool _color);

    /**
     * @brief Construct a custom queen
     */
    Queen(bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the queen
     */
    ~Queen();

    /**
     * @brief Move the queen
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @return true if the queen has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y);

    /**
     * @brief Get the value of the queen
     * @return The value of the queen
     */
    virtual int getValue(void) const;

    /**
     * @brief Print queen
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream &os) const;

    /**
     * @brief Overloading the `<<` operator for queen class
     * @param[in,out] os Output stream
     * @param[in] queen Queen object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Queen const& queen);
};

#endif /* _QUEEN_HPP_ */
