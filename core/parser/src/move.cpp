/**
 * @file  move.cpp
 * @brief Move pieces sources
 */

#include <iostream>
#include "move.hpp"

using namespace std;

Move::Move(string const& _move)
    : move(_move)
{
}

void
Move::print(std::ostream& os) const
{
    os << move;
}

std::ostream&
operator<<(std::ostream& os, Move const& move)
{
    move.print(os);
    return os;
}
