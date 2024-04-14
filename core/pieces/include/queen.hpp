/**
 * @file  queen.hpp
 * @brief Queen header
 */

#ifndef _QUEEN_HPP_
#define _QUEEN_HPP_

#include "piece.hpp"
#include "square.hpp"

/**
 * @brief Queen default value
 */
#define QUEEN_VALUE           (9)
#define QUEEN_BLACK_DEFAULT_X (SQUARE_X_D)
#define QUEEN_BLACK_DEFAULT_Y (SQUARE_Y_8)
#define QUEEN_WHITE_DEFAULT_X (SQUARE_X_D)
#define QUEEN_WHITE_DEFAULT_Y (SQUARE_Y_1)

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
     * @brief Clone a queen
     */
    Queen* clone(void) const
    {
        return new Queen(*this);
    }

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool isAbleToMove(int _x, int _y, int flags, Square* board[8U][8U] = nullptr) const;

    /**
     * @brief Move the queen
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the queen has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int flags);

    /**
     * @brief Get the value of the queen
     * @return The value of the queen
     */
    virtual SquarePieceValue getValue(void) const;

    /**
     * @brief Print queen
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for queen class
     * @param[in,out] os Output stream
     * @param[in] queen Queen object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Queen const& queen);
};

#endif /* _QUEEN_HPP_ */
