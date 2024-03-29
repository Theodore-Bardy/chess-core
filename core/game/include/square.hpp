/**
 * @file  square.hpp
 * @brief Square header
 */

#ifndef _SQUARE_HPP_
#define _SQUARE_HPP_

class Board;
#include "board.hpp"
class Piece;
#include "piece.hpp"
#include <iostream>

class Square
{
private:
    Board* board; /**< Pointer on the parent board */
    Piece* piece; /**< Pointer on the piece */
    bool   color; /**< Color true = withe, false = black */
    int    x;     /**< X position */
    int    y;     /**< Y position */

public:
    /**
     * @brief Construct a new empty square
     * @param[in] _x X square position
     * @param[in] _y Y square position
     */
    Square(Board* _board, int _x, int _y);

    /**
     * @brief Destroy the square
     */
    ~Square();

    /**
     * @brief Place piece in the square
     * @param[in] _piece Piece to place in the square
     */
    void setPiece(Piece* _piece);

    /**
     * @brief Indicate if a piece is in the square
     * @return true if a piece is in the square, false otherwise
     */
    bool hasPiece();

    /**
     * @brief Removed the piece is the square
     * @note This function is safe to be call on an empty square
     * @return true if the piece have been removed, false otherwise
     */
    bool removePiece();

    /**
     * @brief Get the piece in the square
     * @return The piece in the square
     */
    Piece* getPiece();

    /**
     * @brief Check if the square is attacked by a piece of a given color
     * @param _color Color to check for attack
     * @return true is the square is attacked, false otherwise
     */
    bool isAttacked(bool _color);

    /**
     * @brief Returns the current X position
     * @return X position of the square
     */
    int getX(void);

    /**
     * @brief Returns the current Y position
     * @return Y position of the square
     */
    int getY(void);

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

inline int
Square::getX(void)
{
    return x;
}

inline int
Square::getY(void)
{
    return y;
}

#endif /* _SQUARE_HPP_ */
