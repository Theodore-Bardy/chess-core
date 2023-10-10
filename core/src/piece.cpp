/**
 * @file      piece.cpp
 * @brief     Piece sources
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

#include "piece.hpp"

piece::piece(bool _isAlive, bool _color, int _x, int _y)
    : isAlive(_isAlive), color(_color), y(_y), x(_x) {}

piece::~piece() {}
