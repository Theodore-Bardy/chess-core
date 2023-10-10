/**
 * @file      rook.hpp
 * @brief     Rook header
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#ifndef _ROOK_HPP_
#define _ROOK_HPP_

#include "piece.hpp"

/**
 * @brief Rook default value
 */
#define ROOK_VALUE             (5)
#define ROOK_1_BLACK_DEFAULT_X (1)
#define ROOK_1_BLACK_DEFAULT_Y (8)
#define ROOK_2_BLACK_DEFAULT_X (8)
#define ROOK_2_BLACK_DEFAULT_Y (8)
#define ROOK_1_WHITE_DEFAULT_X (1)
#define ROOK_1_WHITE_DEFAULT_Y (1)
#define ROOK_2_WHITE_DEFAULT_X (8)
#define ROOK_2_WHITE_DEFAULT_Y (1)

class rook : public piece
{
private:
    bool hasMoved; /**< Indicate if the rook has moved */

public:
    /**
     * @brief Forbid the default constructor
     */
    rook() = delete;

    /**
     * @brief Construct a default rook
     */
    rook(bool _color, int _x, int _y);

    /**
     * @brief Construct a custom rook
     */
    rook(bool _hasMoved, bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the rook
     */
    ~rook();

    /**
     * @brief Castle the rook
     * @param[in] side true means little castle, flase great castle
     * @return true if the rook has castle, false otherwise
     */
    bool castling(bool side);

    /**
     * @brief Move the rook
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @return true if the rook has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y);

    /**
     * @brief Get the value of the rook
     * @return The value of the rook
     */
    virtual int getValue(void) const;

    /**
     * @brief Print rook in the standard output
     */
    virtual void print(std::ostream &os) const;

    /**
     * @brief Overloading the `<<` operator for rook class
     * @param[in, out] os Output stream
     * @param[in] rook Rook object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, rook const& rook);
};

#endif /* _ROOK_HPP_ */
