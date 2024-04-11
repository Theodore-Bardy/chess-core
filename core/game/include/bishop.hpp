/**
 * @file  bishop.hpp
 * @brief Bishop header
 */

#ifndef _BISHOP_HPP_
#define _BISHOP_HPP_

#include "piece.hpp"
#include "square.hpp"

/**
 * @brief Bishop default value
 */
#define BISHOP_VALUE             (3)
#define BISHOP_1_BLACK_DEFAULT_X (SQUARE_X_C)
#define BISHOP_1_BLACK_DEFAULT_Y (SQUARE_Y_8)
#define BISHOP_2_BLACK_DEFAULT_X (SQUARE_X_F)
#define BISHOP_2_BLACK_DEFAULT_Y (SQUARE_Y_8)
#define BISHOP_1_WHITE_DEFAULT_X (SQUARE_X_C)
#define BISHOP_1_WHITE_DEFAULT_Y (SQUARE_Y_1)
#define BISHOP_2_WHITE_DEFAULT_X (SQUARE_X_F)
#define BISHOP_2_WHITE_DEFAULT_Y (SQUARE_Y_1)

class Bishop : public Piece
{
public:
    /**
     * @brief Forbid the default constructor
     */
    Bishop() = delete;

    /**
     * @brief Construct a default bishop
     */
    Bishop(bool _color, int _x, int _y);

    /**
     * @brief Construct a custom bishop
     */
    Bishop(bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the bishop
     */
    ~Bishop();

    /**
     * @brief Clone a bishop
     */
    Bishop* clone(void) const
    {
        return new Bishop(*this);
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
     * @brief Move the bishop
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the bishop has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int flags);

    /**
     * @brief Get the value of the bishop
     * @return The value of the bishop
     */
    virtual SquareValue getValue(void) const;

    /**
     * @brief Print bishop
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for bishop class
     * @param[in, out] os Output stream
     * @param[in] bishop Bishop object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Bishop const& bishop);
};

#endif /* _BISHOP_HPP_ */
