/**
 * @file  rook.hpp
 * @brief Rook header
 */

#ifndef _ROOK_HPP_
#define _ROOK_HPP_

#include "piece.hpp"
#include "square.hpp"

/**
 * @brief Rook default value
 */
#define ROOK_VALUE             (5)
#define ROOK_1_BLACK_DEFAULT_X (SQUARE_X_A)
#define ROOK_1_BLACK_DEFAULT_Y (SQUARE_Y_8)
#define ROOK_2_BLACK_DEFAULT_X (SQUARE_X_H)
#define ROOK_2_BLACK_DEFAULT_Y (SQUARE_Y_8)
#define ROOK_1_WHITE_DEFAULT_X (SQUARE_X_A)
#define ROOK_1_WHITE_DEFAULT_Y (SQUARE_Y_1)
#define ROOK_2_WHITE_DEFAULT_X (SQUARE_X_H)
#define ROOK_2_WHITE_DEFAULT_Y (SQUARE_Y_1)
#define ROOK_KING_CASTLE_X     (SQUARE_X_F)
#define ROOK_QUEEN_CASTLE_X    (SQUARE_X_D)

class Rook : public Piece
{
private:
    bool hasMoved;   /**< Indicate if the rook has moved */
    bool isKingSide; /**< Indicate if the rook is the king side one */

public:
    /**
     * @brief Forbid the default constructor
     */
    Rook() = delete;

    /**
     * @brief Construct a default rook
     */
    Rook(bool _color, int _x, int _y);

    /**
     * @brief Destroy the rook
     */
    ~Rook() = default;

    /**
     * @brief Clone a rook
     */
    Rook* clone(void) const
    {
        return new Rook(*this);
    }

    /**
     * @brief Castle the rook
     * @return true if the rook has castle, false otherwise
     */
    bool castle(void);

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @param[in] board The board context
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool checkMove(int _x, int _y, int& flags, Square* board[8U][8U]) const;

    /**
     * @brief Move the rook
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @param[in] board The board context
     * @return true if the rook has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int& flags, Square* board[8U][8U]);

    /**
     * @brief Indicate if the rook is the king side one
     * @return true if the rook is the king side one, false otherwise
     */
    bool isKingSideRook(void) const
    {
        return isKingSide;
    }

    /**
     * @brief Get the value of the rook
     * @return The value of the rook
     */
    virtual SquarePieceValue getValue(void) const
    {
        return SquarePieceValue::RookValue;
    }

    /**
     * @brief Print rook
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for rook class
     * @param[in, out] os Output stream
     * @param[in] rook Rook object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Rook const& rook);
};

#endif /* _ROOK_HPP_ */
