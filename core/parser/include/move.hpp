/**
 * @file  move.hpp
 * @brief Move pieces header
 */

#ifndef _MOVE_HPP_
#define _MOVE_HPP_

#include <string>

using namespace std;

class Move
{
private:
    string move;

public:

    /**
     * @brief Construct a default rook
     */
    Move(string const& _move);

    /**
     * @brief Returns the move attribute
     * @return move attribute
     */
    string getMove(void)
    {
        return move;
    }

    /**
     * @brief Print move
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for move class
     * @param[in, out] os Output stream
     * @param[in] move Move object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Move const& move);
};

#endif /* _MOVE_HPP_ */
