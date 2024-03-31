/**
 * @file  game.hpp
 * @brief Chess game header
 */

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <string>
#include <vector>

#include "move.hpp"

using namespace std;

class Game
{
private:
    vector<Move> gameDescription;
    int          index;

public:
    /**
     * @brief Default constructor
     */
    Game();

    /**
     * @brief Default destructor
     */
    ~Game() = default;

    /**
     * @brief Add a move into the gameDescription
     * @param[in] new_move New move to add
     */
    void addMove(Move const& new_move);

    /**
     * @brief Returns the first move of the game
     * @return First move of the game
     */
    Move& firstMove(void);

    /**
     * @brief Returns the previous move depending on the index
     * @return Previous move
     */
    Move& previousMove(void);

    /**
     * @brief Returns the next move depending on the index
     * @return Next move
     */
    Move& nextMove(void);

    /**
     * @brief Returns the last move of the game
     * @return Last move of the game
     */
    Move& lastMove(void);

    /**
     * @brief Returns the index attribute
     * @return index attribute
     */
    int getIndex(void)
    {
        return index;
    }

    /**
     * @brief Print game
     * @param[in, out] os Output stream
     */
    virtual void print(std::ostream& os) const;

    /**
     * @brief Overloading the `<<` operator for game class
     * @param[in, out] os Output stream
     * @param[in] game Game object
     * @return ostream& Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, Game const& game);
};

#endif /* _GAME_HPP_ */
