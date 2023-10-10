/**
 * @file      king.hpp
 * @brief     King header
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#ifndef _KING_HPP_
#define _KING_HPP_

#include "piece.hpp"

/**
 * @brief King default value
 */
#define KING_VALUE           (0)
#define KING_BLACK_DEFAULT_X (5)
#define KING_BLACK_DEFAULT_Y (8)
#define KING_WHITE_DEFAULT_X (5)
#define KING_WHITE_DEFAULT_Y (1)

class king : public piece
{
private:
    bool isCheck;  /**< King check status */
    bool hasMoved; /**< Indicate if the king has moved */

public:
    /**
     * @brief Forbid the default constructor
     */
    king() = delete;

    /**
     * @brief Construct a default king
     */
    king(bool _color);

    /**
     * @brief Construct a custom king
     */
    king(bool _isCheck, bool _hasMoved, bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the king
     */
    ~king();

    /**
     * @brief Castle the king
     * @param[in] side true means little castle, flase great castle
     * @return true if the king has castle, false otherwise
     */
    bool castling(bool side);

    /**
     * @brief Move the king
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @return true if the king has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y);

    /**
     * @brief Get the value of the king
     * @return The value of the king
     */
    virtual int getValue(void) const;

    /**
     * @brief Print king in the standard output
     */
    virtual void print(std::ostream &os) const;

    /**
     * @brief Overloading the `<<` operator for king class
     * @param[in, out] os Output stream
     * @param[in] king King object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, king const& king);
};

#endif /* _KING_HPP_ */
