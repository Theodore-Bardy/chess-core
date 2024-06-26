/**
 * @file  square.hpp
 * @brief Square header
 */

#ifndef _SQUARE_HPP_
#define _SQUARE_HPP_

#include <iostream>

/**
 * @brief X coordinates
 */
#define SQUARE_X_A (0)
#define SQUARE_X_B (1)
#define SQUARE_X_C (2)
#define SQUARE_X_D (3)
#define SQUARE_X_E (4)
#define SQUARE_X_F (5)
#define SQUARE_X_G (6)
#define SQUARE_X_H (7)

/**
 * @brief Y coordinates
 */
#define SQUARE_Y_1 (0)
#define SQUARE_Y_2 (1)
#define SQUARE_Y_3 (2)
#define SQUARE_Y_4 (3)
#define SQUARE_Y_5 (4)
#define SQUARE_Y_6 (5)
#define SQUARE_Y_7 (6)
#define SQUARE_Y_8 (7)

/**
 * @brief Square piece color values list
 */
enum SquarePieceColor
{
    NoPiece,
    White,
    Black
};

/**
 * @brief Square values list
 */
enum SquarePieceValue
{
    Empty,
    KingValue,
    QueenValue,
    BishopValue,
    KnightValue,
    RookValue,
    PawnValue
};

class Square
{
private:
    SquarePieceColor color; /**< Color of the piece in the square (0 = no piece, 1 = white, 2 = black) */
    SquarePieceValue value; /**< Value of the piece in the square (cf. SquarePieceValue) */
    int              x;     /**< X position */
    int              y;     /**< Y position */

public:
    /**
     * @brief Construct a new empty square
     * @param[in] _x X square position
     * @param[in] _y Y square position
     */
    Square(int _x, int _y);

    /**
     * @brief Destroy the square
     */
    ~Square() = default;

    /**
     * @brief Get the Value object
     * @return Value of the square (cf. SquarePieceValue)
     */
    SquarePieceColor getColor(void) const
    {
        return color;
    }

    /**
     * @brief Set the color of the piece in the square
     * @param[in] _color Color of the piece in the square
     */
    void setColor(SquarePieceColor _color)
    {
        color = _color;
    }

    /**
     * @brief Get the value of the square
     * @return Value of the square (cf. SquarePieceValue)
     */
    SquarePieceValue getValue(void) const
    {
        return value;
    }

    /**
     * @brief Set the value of the square
     * @param[in] _value Value of the square
     */
    void setValue(SquarePieceValue _value)
    {
        value = _value;
    }

    /**
     * @brief Returns the current X position
     * @return X position of the square
     */
    int getX(void) const
    {
        return x;
    }

    /**
     * @brief Returns the current Y position
     * @return Y position of the square
     */
    int getY(void) const
    {
        return y;
    }

    /**
     * @brief Reset square as default (no piece)
     */
    void resetSquare(void)
    {
        color = SquarePieceColor::NoPiece;
        value = SquarePieceValue::Empty;
    }

    /**
     * @brief Print square
     * @param[in, out] os Output stream
     */
    void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for square class
     * @param[in, out] os Output stream
     * @param[in] square Square object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Square const& square);
};

#endif /* _SQUARE_HPP_ */
