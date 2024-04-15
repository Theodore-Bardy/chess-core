/**
 * @file  knight.hpp
 * @brief Knight header
 */

#ifndef _KNIGHT_HPP_
#define _KNIGHT_HPP_

#include "piece.hpp"
#include "square.hpp"

/**
 * @brief Knight default value
 */
#define KNIGHT_VALUE             (3)
#define KNIGHT_1_BLACK_DEFAULT_X (SQUARE_X_B)
#define KNIGHT_1_BLACK_DEFAULT_Y (SQUARE_Y_8)
#define KNIGHT_2_BLACK_DEFAULT_X (SQUARE_X_G)
#define KNIGHT_2_BLACK_DEFAULT_Y (SQUARE_Y_8)
#define KNIGHT_1_WHITE_DEFAULT_X (SQUARE_X_B)
#define KNIGHT_1_WHITE_DEFAULT_Y (SQUARE_Y_1)
#define KNIGHT_2_WHITE_DEFAULT_X (SQUARE_X_G)
#define KNIGHT_2_WHITE_DEFAULT_Y (SQUARE_Y_1)

class Knight : public Piece
{
public:
    /**
     * @brief Forbid the default constructor
     */
    Knight() = delete;

    /**
     * @brief Construct a default knight
     */
    Knight(bool _color, int _x, int _y);

    /**
     * @brief Construct a custom knight (use only for pawn promotion)
     */
    Knight(bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the knight
     */
    ~Knight() = default;

    /**
     * @brief Clone a knight
     */
    Knight* clone(void) const
    {
        return new Knight(*this);
    }

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @param[in] board Give more context to check the move (optional)
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool checkMove(int _x, int _y, int flags, Square* board[8U][8U] = nullptr) const;

    /**
     * @brief Move the knight
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the knight has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int flags);

    /**
     * @brief Get the value of the knight
     * @return The value of the knight
     */
    virtual SquarePieceValue getValue(void) const
    {
        return SquarePieceValue::KnightValue;
    }

    /**
     * @brief Print knight
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for knight class
     * @param[in, out] os Output stream
     * @param[in] knight Knight object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Knight const& knight);
};

#endif /* _KNIGHT_HPP_ */
