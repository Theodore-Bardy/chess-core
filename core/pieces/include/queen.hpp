/**
 * @file  queen.hpp
 * @brief Queen header
 */

#ifndef _QUEEN_HPP_
#define _QUEEN_HPP_

#include "piece.hpp"

/**
 * @brief Queen default value
 */
#define QUEEN_VALUE           (9)
#define QUEEN_BLACK_DEFAULT_X (X_D)
#define QUEEN_BLACK_DEFAULT_Y (Y_8)
#define QUEEN_WHITE_DEFAULT_X (X_D)
#define QUEEN_WHITE_DEFAULT_Y (Y_1)

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
     * @brief Construct a custom queen (use only for pawn promotion)
     */
    Queen(bool _color, int _x, int _y);

    /**
     * @brief Destroy the queen
     */
    ~Queen() = default;

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool checkMove(int _x, int _y, int& flags) const;

    /**
     * @brief Move the queen
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the queen has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int& flags);

    /**
     * @brief Get the piece type
     * @return The piece type
     */
    virtual PieceType getType(void) const
    {
        return PieceType::QueenType;
    }

    /**
     * @brief Get the piece value
     * @return The piece value
     */
    virtual int getValue(void) const
    {
        return QUEEN_VALUE;
    }

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
