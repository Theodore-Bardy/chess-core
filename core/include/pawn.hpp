/**
 * @file      pawn.hpp
 * @brief     Pawn header
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "piece.hpp"

/**
 * @brief Pawn default value
 */
#define PAWN_VALUE           (1)
#define PAWN_BLACK_DEFAULT_Y (7)
#define PAWN_WHITE_DEFAULT_Y (2)

class pawn : public piece
{
private:
    bool hasMoved; /**< Indicate if the pawn has moved */

public:
    /**
     * @brief Forbid the default constructor
     */
    pawn() = delete;

    /**
     * @brief Construct a default pawn
     */
    pawn(bool _color, int _x);

    /**
     * @brief Construct a custom pawn
     */
    pawn(bool _hasMoved, bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the pawn
     */
    ~pawn();

    /**
     * @brief Promote the pawn
     */
    void promotion();

    /**
     * @brief Move the pawn
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @return true if the pawn has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y);

    /**
     * @brief Get the value of the pawn
     * @return The value of the pawn
     */
    virtual int getValue(void) const;

    /**
     * @brief Print pawn in the standard output
     */
    virtual void print(std::ostream &os) const;

    /**
     * @brief Overloading the `<<` operator for pawn class
     * @param[in, out] os Output stream
     * @param[in] pawn Pawn object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, pawn const& pawn);
};

#endif /* _PAWN_HPP_ */
