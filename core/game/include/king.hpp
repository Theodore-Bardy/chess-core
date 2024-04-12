/**
 * @file  king.hpp
 * @brief King header
 */

#ifndef _KING_HPP_
#define _KING_HPP_

#include "piece.hpp"
#include "square.hpp"

/**
 * @brief King default value
 */
#define KING_VALUE           (0)
#define KING_BLACK_DEFAULT_X (SQUARE_X_E)
#define KING_BLACK_DEFAULT_Y (SQUARE_Y_8)
#define KING_WHITE_DEFAULT_X (SQUARE_X_E)
#define KING_WHITE_DEFAULT_Y (SQUARE_Y_1)
#define KING_KING_CASTLE_X   (SQUARE_X_G)
#define KING_QUEEN_CASTLE_X  (SQUARE_X_C)

class King : public Piece
{
private:
    bool isCheck;  /**< King check status */
    bool hasMoved; /**< Indicate if the king has moved */

public:
    /**
     * @brief Forbid the default constructor
     */
    King() = delete;

    /**
     * @brief Construct a default king
     */
    King(bool _color);

    /**
     * @brief Construct a custom king
     */
    King(bool _isCheck, bool _hasMoved, bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the king
     */
    ~King();

    /**
     * @brief Clone a king
     */
    King* clone(void) const
    {
        return new King(*this);
    }

    /**
     * @brief Castle the king
     * @param[in] side true means little castle, flase great castle
     * @return true if the king has castle, false otherwise
     */
    bool castling(bool side);

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool isAbleToMove(int _x, int _y, int flags, Square* board[8U][8U] = nullptr) const;

    /**
     * @brief Move the king
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the king has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int flags);

    /**
     * @brief Get the value of the king
     * @return The value of the king
     */
    virtual SquarePieceValue getValue(void) const;

    /**
     * @brief Print king
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for king class
     * @param[in, out] os Output stream
     * @param[in] king King object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, King const& king);
};

#endif /* _KING_HPP_ */
