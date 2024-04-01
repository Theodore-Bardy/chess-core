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

class Rook : public Piece
{
private:
    bool hasMoved; /**< Indicate if the rook has moved */

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
     * @brief Construct a custom rook
     */
    Rook(bool _hasMoved, bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Destroy the rook
     */
    ~Rook();

    /**
     * @brief Clone a rook
     */
    Rook* clone(void) const
    {
        return new Rook(*this);
    }

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
    virtual SquareValue getValue(void) const;

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
