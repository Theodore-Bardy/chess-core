/**
 * @file  king.hpp
 * @brief King header
 */

#ifndef _KING_HPP_
#define _KING_HPP_

#include "piece.hpp"
#include "rook.hpp"

/**
 * @brief King default value
 */
#define KING_VALUE           (0)
#define KING_BLACK_DEFAULT_X (X_E)
#define KING_BLACK_DEFAULT_Y (Y_8)
#define KING_WHITE_DEFAULT_X (X_E)
#define KING_WHITE_DEFAULT_Y (Y_1)
#define KING_KING_CASTLE_X   (X_G)
#define KING_QUEEN_CASTLE_X  (X_C)

/**
 * @brief Types of castles
 */
#define KING_CASTLE  (true)
#define QUEEN_CASTLE (false)

class King : public Piece
{
private:
    bool isCheck;  /**< King check status */
    bool isMate;   /**< King check mate status */
    bool isPat;    /**< King pat status */
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
     * @brief Destroy the king
     */
    ~King() = default;

    /**
     * @brief Castle the king
     * @param[in] side true means king castle, false queen castle
     * @param[in] rook Rook which castles with the king
     * @return true if the king has castle, false otherwise
     */
    bool castle(bool side, Rook& rook);

    /**
     * @brief Check if the piece is able to move a the desired position
     * @param[in] _x Desired X position
     * @param[in] _y Desired Y position
     * @param[in] flags Move flags
     * @return true if the piece is able to move, otherwise false
     */
    virtual bool checkMove(int _x, int _y, int& flags) const;

    /**
     * @brief Move the king
     * @param[in] x Desired X position
     * @param[in] y Desired Y position
     * @param[in] flags Move flags
     * @return true if the king has moved to the desired destination, false otherwise
     */
    virtual bool move(int _x, int _y, int& flags);

    /**
     * @brief Get the piece type
     * @return The piece type
     */
    virtual PieceType getType(void) const
    {
        return PieceType::KingType;
    }

    /**
     * @brief Get the piece value
     * @return The piece value
     */
    virtual int getValue(void) const
    {
        return KING_VALUE;
    }

    /**
     * @brief Get the king check status
     * @return The king check status
     */
    bool getCheckStatus(void) const
    {
        return isCheck;
    }

    /**
     * @brief Set the king check state
     * @param[in] _isCheck New king check state
     */
    void setCheckStatus(bool _isCheck)
    {
        isCheck = _isCheck;
    }

    /**
     * @brief Get the king check status
     * @return The king check status
     */
    bool getMateStatus(void) const
    {
        return isMate;
    }

    /**
     * @brief Set the king mate state
     * @param[in] _isCheck New king mate state
     */
    void setMateStatus(bool _isMate)
    {
        isMate = _isMate;
    }

    /**
     * @brief Get the king check status
     * @return The king check status
     */
    bool getPatStatus(void) const
    {
        return isPat;
    }

    /**
     * @brief Set the king pat state
     * @param[in] _isCheck New king pat state
     */
    void setPatStatus(bool _isPat)
    {
        isPat = _isPat;
    }

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
