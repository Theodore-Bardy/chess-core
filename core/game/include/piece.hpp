/**
 * @file  piece.hpp
 * @brief Piece header
 */

#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include <iostream>

class Piece
{
#define PIECE_WHITE (true)
#define PIECE_BLACK (false)

protected:
    bool isAlive; /**< Alive state */
    bool color;   /**< Color true = withe, false = black */
    int  x;       /**< X position */
    int  y;       /**< Y position */

public:
    /**
     * @brief Forbid the default constructor
     */
    Piece() = delete;

    /**
     * @brief Construct a new piece
     * @param[in] _color Color of the piece
     * @param[in] _x Default X position
     * @param[in] _y Default Y position
     */
    Piece(bool _color, int _x, int _y);

    /**
     * @brief Construct a new piece
     * @param[in] _isAlive Alive state
     * @param[in] _color Color of the piece
     * @param[in] _x Default X position
     * @param[in] _y Default Y position
     */
    Piece(bool _isAlive, bool _color, int _x, int _y);

    /**
     * @brief Copy a piece
     * @param piece Piece to copy
     */
    Piece(Piece const& piece);

    /**
     * @brief Destroy the piece
     */
    ~Piece();

    /**
     * @brief Eat a piece
     * @param[in, out] piece_to_eat Piece to eat
     */
    void eat(Piece& piece_to_eat);

    /**
     * @brief Returns the current X position
     * @return X position of the piece 
     */
    int getX(void);

    /**
     * @brief Returns the current Y position
     * @return Y position of the piece 
     */
    int getY(void);

    /**
     * @brief Move a piece
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @return true if the piece has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y) = 0;

    /**
     * @brief Get the value of the piece
     * @return The value of the piece
     */
    virtual int getValue(void) const = 0;

    /**
     * @brief Print the piece
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const = 0;
};

inline int
Piece::getX(void)
{
    return x;
}

inline int
Piece::getY(void)
{
    return y;
}

#endif /* _PIECE_HPP_ */
