/**
 * @file  pawn.hpp
 * @brief Pawn header
 */

#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "piece.hpp"
#include "square.hpp"

/**
 * @brief Pawn default value
 */
#define PAWN_VALUE           (1)
#define PAWN_BLACK_DEFAULT_Y (SQUARE_Y_7)
#define PAWN_WHITE_DEFAULT_Y (SQUARE_Y_2)

class Pawn : public Piece
{
private:
    bool hasMoved; /**< Indicate if the pawn has moved */

public:
    /**
     * @brief Forbid the default constructor
     */
    Pawn() = delete;

    /**
     * @brief Construct a default pawn
     */
    Pawn(bool _color, int _x);

    /**
     * @brief Destroy the pawn
     */
    ~Pawn() = default;

    /**
     * @brief Clone a pawn
     */
    Pawn* clone(void) const
    {
        return new Pawn(*this);
    }

    /**
     * @brief Promote the pawn
     */
    void promotion();

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @param[in] board Give more context to check the move (optional)
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool checkMove(int _x, int _y, int& flags, Square* board[8U][8U]) const;

    /**
     * @brief Move the pawn
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the pawn has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int& flags, Square* board[8U][8U]);

    /**
     * @brief Get the value of the pawn
     * @return The value of the pawn
     */
    virtual SquarePieceValue getValue(void) const
    {
        return SquarePieceValue::PawnValue;
    }

    /**
     * @brief Print pawn
     * @return Pawn as string
     */
    virtual std::string print(void) const;

    /**
     * @brief Overloading the `<<` operator for pawn class
     * @param[in, out] os Output stream
     * @param[in] pawn Pawn object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Pawn const& pawn);
};

#endif /* _PAWN_HPP_ */
