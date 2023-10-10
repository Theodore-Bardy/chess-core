/**
 * @file      piece.hpp
 * @brief     main file
 * @copyright Copyright (C) Theodore Bardy. All rights reserved.
 *            Developed by Theodore Bardy.
 *            Reproduction, copy, modification in whole or part is prohibited
 *            without the written permission of the copyright owner.
 */

class piece
{
private:
    bool isAlive;
    bool color;
    int  x;
    int  y;

public:
    piece(bool _isAlive, bool _color, int _x, int _y);
    ~piece();
};
