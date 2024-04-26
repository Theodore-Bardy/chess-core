/**
 * @file  queen_test.cpp
 * @brief Queen sources unit tests
 */

#include <gtest/gtest.h>
#include "queen.hpp"

TEST(QueenTest, Constructor)
{
    Queen q1(PIECE_WHITE);
    EXPECT_EQ(q1.getColor(), PIECE_WHITE);
    EXPECT_EQ(q1.getX(), X_D);
    EXPECT_EQ(q1.getY(), Y_1);

    Queen q2(PIECE_BLACK);
    EXPECT_EQ(q2.getColor(), PIECE_BLACK);
    EXPECT_EQ(q2.getX(), X_D);
    EXPECT_EQ(q2.getY(), Y_8);
}

TEST(QueenTest, CheckMove)
{
    Queen q(PIECE_WHITE);
    int   flags = 0;

    /* 8| .  .  .  ×  .  .  .  .
     * 7| .  .  .  ×  .  .  .  .
     * 6| .  .  .  ×  .  .  .  .
     * 5| .  .  .  ×  .  .  .  ×
     * 4| ×  .  .  ×  .  .  ×  .
     * 3| .  ×  .  ×  .  ×  .  .
     * 2| .  .  ×  ×  ×  .  .  .
     * 1| ×  ×  ×  Q  ×  ×  ×  × 
     * ___________________________
     *  | a  b  c  d  e  f  g  h 
     */

    /* Able to move to diagonals and rows */
    EXPECT_EQ(q.checkMove(X_A, Y_1, flags), true);
    EXPECT_EQ(q.checkMove(X_A, Y_4, flags), true);
    EXPECT_EQ(q.checkMove(X_B, Y_1, flags), true);
    EXPECT_EQ(q.checkMove(X_B, Y_3, flags), true);
    EXPECT_EQ(q.checkMove(X_C, Y_1, flags), true);
    EXPECT_EQ(q.checkMove(X_C, Y_2, flags), true);
    EXPECT_EQ(q.checkMove(X_D, Y_2, flags), true);
    EXPECT_EQ(q.checkMove(X_D, Y_3, flags), true);
    EXPECT_EQ(q.checkMove(X_D, Y_4, flags), true);
    EXPECT_EQ(q.checkMove(X_D, Y_5, flags), true);
    EXPECT_EQ(q.checkMove(X_D, Y_6, flags), true);
    EXPECT_EQ(q.checkMove(X_D, Y_7, flags), true);
    EXPECT_EQ(q.checkMove(X_D, Y_8, flags), true);
    EXPECT_EQ(q.checkMove(X_E, Y_1, flags), true);
    EXPECT_EQ(q.checkMove(X_E, Y_2, flags), true);
    EXPECT_EQ(q.checkMove(X_F, Y_1, flags), true);
    EXPECT_EQ(q.checkMove(X_F, Y_3, flags), true);
    EXPECT_EQ(q.checkMove(X_G, Y_1, flags), true);
    EXPECT_EQ(q.checkMove(X_G, Y_4, flags), true);
    EXPECT_EQ(q.checkMove(X_H, Y_1, flags), true);
    EXPECT_EQ(q.checkMove(X_H, Y_5, flags), true);

    /* Otherwise unable to move - random values */
    EXPECT_EQ(q.checkMove(X_A, Y_8, flags), false);
    EXPECT_EQ(q.checkMove(X_D, Y_1, flags), false);
    EXPECT_EQ(q.checkMove(X_A, Y_2, flags), false);
    EXPECT_EQ(q.checkMove(X_E, Y_4, flags), false);
}

TEST(QueenTest, Move)
{
    Queen q(PIECE_WHITE);
    int   flags = 0;

    /* Unable to move on its position */
    EXPECT_EQ(q.move(X_D, Y_1, flags), false);
    EXPECT_EQ(q.getX(), X_D);
    EXPECT_EQ(q.getY(), Y_1);

    /* Unable to move - random value */
    EXPECT_EQ(q.move(X_A, Y_7, flags), false);
    EXPECT_EQ(q.getX(), X_D);
    EXPECT_EQ(q.getY(), Y_1);

    /* Able to move to diagonals and rows */
    EXPECT_EQ(q.move(X_D, Y_5, flags), true);
    EXPECT_EQ(q.getX(), X_D);
    EXPECT_EQ(q.getY(), Y_5);
}

TEST(QueenTest, GetType)
{
    Queen q(PIECE_WHITE);
    EXPECT_EQ(q.getType(), PieceType::QueenType);
}

TEST(QueenTest, GetValue)
{
    Queen q(PIECE_WHITE);
    EXPECT_EQ(q.getValue(), QUEEN_VALUE);
}
