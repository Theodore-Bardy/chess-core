/**
 * @file  piece.hpp
 * @brief Piece header
 */

#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include <iostream>

#include "square.hpp"

class Piece
{
#define PIECE_WHITE (true)
#define PIECE_BLACK (false)

protected:
    bool alive; /**< Alive state */
    bool color; /**< Color true = white, false = black */
    int  x;     /**< X position */
    int  y;     /**< Y position */

    /**
     * @brief Check the way between current and desired position excluded is empty
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] board Context board
     * @return true if the way is empty, otherwise false
     */
    bool checkWayOnMove(int _x, int _y, Square* board[8U][8U]) const;

    /**
     * @brief Check desired position isn't a piece of the same color
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] board Context board
     * @return -1 if there is piece with the same color at the desired position, 0 if there is no piece, 1 if there is
     *         piece of the opposite color
     */
    int checkFinalOnMove(int _x, int _y, Square* board[8U][8U]) const;

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
     * @brief Destroy the piece
     */
    ~Piece() = default;

    /**
     * @brief Clone a piece
     */
    virtual Piece* clone(void) const = 0;

    /**
     * @brief Take a piece
     * @param[in] piece_to_take Piece to take
     */
    void take(Piece& piece_to_take);

    /**
     * @brief Returns the color of the piece
     * @return Color of the piece
     */
    bool getColor(void) const;

    /**
     * @brief Returns the life status
     * @return Life status the piece
     */
    bool isAlive(void) const;

    /**
     * @brief Returns the current X position
     * @return X position of the piece 
     */
    int getX(void) const;

    /**
     * @brief Returns the current Y position
     * @return Y position of the piece 
     */
    int getY(void) const;

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @param[in] board Give more context to check the move (optional)
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool checkMove(int _x, int _y, int& flags, Square* board[8U][8U]) const = 0;

    /**
     * @brief Move a piece
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the piece has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int& flags, Square* board[8U][8U]) = 0;

    /**
     * @brief Get the value of the piece
     * @return The value of the piece
     */
    virtual SquarePieceValue getValue(void) const = 0;

    /**
     * @brief Print the piece
     * @return Piece as string
     */
    virtual std::string print(void) const = 0;

    /**
     * @brief Overloading the `<<` operator for piece class
     * @param[in, out] os Output stream
     * @param[in] piece Piece object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Piece const& piece);
};

inline bool
Piece::getColor(void) const
{
    return color;
}

inline bool
Piece::isAlive(void) const
{
    return alive;
}

inline int
Piece::getX(void) const
{
    return x;
}

inline int
Piece::getY(void) const
{
    return y;
}

#endif /* _PIECE_HPP_ */
