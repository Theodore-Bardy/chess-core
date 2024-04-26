/**
 * @file  piece.hpp
 * @brief Piece header
 */

#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include <iostream>
#include <vector>

/**
 * @brief X coordinates
 */
#define X_A (0)
#define X_B (1)
#define X_C (2)
#define X_D (3)
#define X_E (4)
#define X_F (5)
#define X_G (6)
#define X_H (7)

/**
 * @brief Y coordinates
 */
#define Y_1 (0)
#define Y_2 (1)
#define Y_3 (2)
#define Y_4 (3)
#define Y_5 (4)
#define Y_6 (5)
#define Y_7 (6)
#define Y_8 (7)

/**
 * @brief Optional flag on a move
 */
#define MOVE_FLAG_TAKE         (0x01) /**< (0b0000_0001) */
#define MOVE_FLAG_KING_CASTLE  (0x02) /**< (0b0000_0010) */
#define MOVE_FLAG_QUEEN_CASTLE (0x04) /**< (0b0000_0100) */
#define MOVE_FLAG_CHECK        (0x08) /**< (0b0000_1000) */
#define MOVE_FLAG_CHECK_MATE   (0x10) /**< (0b0001_0000) */
#define MOVE_FLAG_PROMOTION    (0x20) /**< (0b0010_0000) */

/**
 * @brief Piece color types
 */
#define PIECE_WHITE (true)
#define PIECE_BLACK (false)

/**
 * @brief Piece type list
 */
enum PieceType
{
    KingType,
    QueenType,
    BishopType,
    KnightType,
    RookType,
    PawnType
};

class Piece
{
protected:
    bool alive; /**< Alive state */
    bool color; /**< Color true = white, false = black */
    int  x;     /**< X position */
    int  y;     /**< Y position */

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
     * @brief Take a piece
     * @param[in] piece_to_take Piece to take
     */
    bool take(Piece& piece_to_take, int& flags);

    /**
     * @brief Returns all legal moves for a piece
     * @return Vector of (x,y) position
     */
    std::vector<std::pair<int, int>> getAllMoves(void) const;

    /**
     * @brief Returns the color of the piece
     * @return Color of the piece
     */
    bool getColor(void) const
    {
        return color;
    }

    /**
     * @brief Returns the life status
     * @return Life status the piece
     */
    bool isAlive(void) const
    {
        return alive;
    }

    /**
     * @brief Returns the current X position
     * @return X position of the piece 
     */
    int getX(void) const
    {
        return x;
    }

    /**
     * @brief Returns the current Y position
     * @return Y position of the piece 
     */
    int getY(void) const
    {
        return y;
    }

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool checkMove(int _x, int _y, int& flags) const = 0;

    /**
     * @brief Move a piece
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the piece has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int& flags) = 0;

    /**
     * @brief Get the piece type
     * @return The piece type
     */
    virtual PieceType getType(void) const = 0;

    /**
     * @brief Get the piece value
     * @return The piece value
     */
    virtual int getValue(void) const = 0;

    /**
     * @brief Print the piece
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const = 0;

    /**
     * @brief Overloading the `<<` operator for piece class
     * @param[in, out] os Output stream
     * @param[in] piece Piece object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Piece const& piece);
};

#endif /* _PIECE_HPP_ */
