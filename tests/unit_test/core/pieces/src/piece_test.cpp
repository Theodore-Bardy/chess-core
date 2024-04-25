/**
 * @file  piece_test.cpp
 * @brief Piece sources unit tests
 */

#include <gtest/gtest.h>
#include "piece.hpp"
#include "rook.hpp"
#include "square.hpp"

TEST(Piece, GetAllMoves)
{
    /* Initialize board */
    Square* board[8U][8U];
    for (int y = 0; y < 8U; ++y)
    {
        for (int x = 0; x < 8U; ++x)
        {
            board[x][y] = new Square(x, y);
        }
    }

    /* Initialize piece */
    Rook r1(true, SQUARE_X_D, SQUARE_Y_4);
    board[SQUARE_X_D][SQUARE_Y_4]->setColor(SquarePieceColor::White);
    board[SQUARE_X_D][SQUARE_Y_4]->setValue(SquarePieceValue::RookValue);

    board[SQUARE_X_F][SQUARE_Y_4]->setColor(SquarePieceColor::White);
    board[SQUARE_X_F][SQUARE_Y_4]->setValue(SquarePieceValue::RookValue);

    board[SQUARE_X_D][SQUARE_Y_3]->setColor(SquarePieceColor::Black);
    board[SQUARE_X_D][SQUARE_Y_3]->setValue(SquarePieceValue::RookValue);

    /* 8|  .  .  .  ×  .  .  .  . 
     * 7|  .  .  .  ×  .  .  .  . 
     * 6|  .  .  .  ×  .  .  .  . 
     * 5|  .  .  .  ×  .  .  .  . 
     * 4|  ×  ×  ×  ♖ ×  ♖  .  . 
     * 3|  .  .  . ♜× .  .  .  . 
     * 2|  .  .  .  .  .  .  .  . 
     * 1|  .  .  .  .  .  .  .  . 
     * ___________________________
     *  | a  b  c  d  e  f  g  h 
     */

    /* Test function */
    std::vector<std::pair<int, int>> moves = r1.getAllMoves(board);
    EXPECT_EQ(moves.size(), 9U);
    EXPECT_EQ(moves.at(0).first, SQUARE_X_A);
    EXPECT_EQ(moves.at(0).second, SQUARE_Y_4);
    EXPECT_EQ(moves.at(1).first, SQUARE_X_B);
    EXPECT_EQ(moves.at(1).second, SQUARE_Y_4);
    EXPECT_EQ(moves.at(2).first, SQUARE_X_C);
    EXPECT_EQ(moves.at(2).second, SQUARE_Y_4);
    EXPECT_EQ(moves.at(3).first, SQUARE_X_D);
    EXPECT_EQ(moves.at(3).second, SQUARE_Y_3);
    EXPECT_EQ(moves.at(4).first, SQUARE_X_D);
    EXPECT_EQ(moves.at(4).second, SQUARE_Y_5);
    EXPECT_EQ(moves.at(5).first, SQUARE_X_D);
    EXPECT_EQ(moves.at(5).second, SQUARE_Y_6);
    EXPECT_EQ(moves.at(6).first, SQUARE_X_D);
    EXPECT_EQ(moves.at(6).second, SQUARE_Y_7);
    EXPECT_EQ(moves.at(7).first, SQUARE_X_D);
    EXPECT_EQ(moves.at(7).second, SQUARE_Y_8);
    EXPECT_EQ(moves.at(8).first, SQUARE_X_E);
    EXPECT_EQ(moves.at(8).second, SQUARE_Y_4);
}
