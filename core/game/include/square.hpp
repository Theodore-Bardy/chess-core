/**
 * @file  square.hpp
 * @brief Square header
 */

#ifndef _SQUARE_HPP_
#define _SQUARE_HPP_

#include <iostream>

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
enum SquareValue
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
    SquareValue      value; /**< Value of the piece in the square (cf. SquareValue) */
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
     * @return Value of the square (cf. SquareValue)
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
     * @return Value of the square (cf. SquareValue)
     */
    SquareValue getValue(void) const
    {
        return value;
    }

    /**
     * @brief Set the value of the square
     * @param[in] _value Value of the square
     */
    void setValue(SquareValue _value)
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
