/**
 * @file  knight.hpp
 * @brief Knight header
 */

#ifndef _KNIGHT_HPP_
#define _KNIGHT_HPP_

#include "piece.hpp"

/**
 * @brief Knight default value
 */
#define KNIGHT_VALUE             (3)
#define KNIGHT_1_BLACK_DEFAULT_X (X_B)
#define KNIGHT_1_BLACK_DEFAULT_Y (Y_8)
#define KNIGHT_2_BLACK_DEFAULT_X (X_G)
#define KNIGHT_2_BLACK_DEFAULT_Y (Y_8)
#define KNIGHT_1_WHITE_DEFAULT_X (X_B)
#define KNIGHT_1_WHITE_DEFAULT_Y (Y_1)
#define KNIGHT_2_WHITE_DEFAULT_X (X_G)
#define KNIGHT_2_WHITE_DEFAULT_Y (Y_1)

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
     * @brief Destroy the knight
     */
    ~Knight() = default;

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool checkMove(int _x, int _y, int& flags) const;

    /**
     * @brief Move the knight
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the knight has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int& flags);

    /**
     * @brief Get the piece type
     * @return The piece type
     */
    virtual PieceType getType(void) const
    {
        return PieceType::KnightType;
    }

    /**
     * @brief Get the piece value
     * @return The piece value
     */
    virtual int getValue(void) const
    {
        return KNIGHT_VALUE;
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
