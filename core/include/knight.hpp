/**
 * @file      knight.hpp
 * @brief     knight header
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#ifndef _KNIGHT_HPP_
#define _KNIGHT_HPP_

#include "piece.hpp"

/**
 * @brief Knight default value
 */
#define KNIGHT_VALUE             (3)
#define KNIGHT_1_BLACK_DEFAULT_X (2)
#define KNIGHT_1_BLACK_DEFAULT_Y (8)
#define KNIGHT_2_BLACK_DEFAULT_X (7)
#define KNIGHT_2_BLACK_DEFAULT_Y (8)
#define KNIGHT_1_WHITE_DEFAULT_X (2)
#define KNIGHT_1_WHITE_DEFAULT_Y (1)
#define KNIGHT_2_WHITE_DEFAULT_X (7)
#define KNIGHT_2_WHITE_DEFAULT_Y (1)

class knight : public piece
{
public:
    /**
     * @brief Forbid the default constructor
     */
    knight() = delete;

    /**
     * @brief Construct a default knight
     */
    knight(bool _color, int _x, int _y);

    /**
     * @brief Construct a custom knight
     */
    knight(bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the knight
     */
    ~knight();

    /**
     * @brief Move the knight
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @return true if the knight has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y);

    /**
     * @brief Get the value of the knight
     * @return The value of the knight
     */
    virtual int getValue(void) const;

    /**
     * @brief Print knight in the standard output
     */
    virtual void print(std::ostream &os) const;

    /**
     * @brief Overloading the `<<` operator for knight class
     * @param[in, out] os Output stream
     * @param[in] knight Knight object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, knight const& knight);
};

#endif /* _KNIGHT_HPP_ */
