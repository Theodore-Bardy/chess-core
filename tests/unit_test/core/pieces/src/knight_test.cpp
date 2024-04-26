/**
 * @file  knight_test.cpp
 * @brief Knight sources unit tests
 */

#include <gtest/gtest.h>
#include "knight.hpp"

TEST(KnightTest, Constructor)
{
    Knight k(PIECE_WHITE, X_D, Y_6);

    EXPECT_EQ(k.getColor(), PIECE_WHITE);
    EXPECT_EQ(k.getX(), X_D);
    EXPECT_EQ(k.getY(), Y_6);
}

TEST(KnightTest, CheckMove)
{
    Knight k(PIECE_WHITE, X_D, Y_4);
    int    flags = 0;

    /* 8| .  .  .  .  .  .  .  .
     * 7| .  .  .  .  .  .  .  .
     * 6| .  .  o  .  o  .  .  .
     * 5| .  o  .  .  .  o  .  .
     * 4| .  .  .  C  .  .  .  .
     * 3| .  o  .  .  .  o  .  .
     * 2| .  .  o  .  o  .  .  . 
     * 1| .  .  .  .  .  .  .  . 
     * ___________________________
     *  | a  b  c  d  e  f  g  h 
     */

    /* Able to move in L */
    EXPECT_EQ(k.checkMove(X_B, Y_3, flags), true);
    EXPECT_EQ(k.checkMove(X_B, Y_5, flags), true);
    EXPECT_EQ(k.checkMove(X_C, Y_2, flags), true);
    EXPECT_EQ(k.checkMove(X_C, Y_6, flags), true);
    EXPECT_EQ(k.checkMove(X_E, Y_2, flags), true);
    EXPECT_EQ(k.checkMove(X_E, Y_6, flags), true);
    EXPECT_EQ(k.checkMove(X_F, Y_3, flags), true);
    EXPECT_EQ(k.checkMove(X_F, Y_5, flags), true);

    /* Otherwise to move - random values */
    EXPECT_EQ(k.checkMove(X_B, Y_4, flags), false);
    EXPECT_EQ(k.checkMove(X_C, Y_4, flags), false);
    EXPECT_EQ(k.checkMove(X_C, Y_5, flags), false);
    EXPECT_EQ(k.checkMove(X_D, Y_2, flags), false);
    EXPECT_EQ(k.checkMove(X_D, Y_5, flags), false);
    EXPECT_EQ(k.checkMove(X_F, Y_6, flags), false);
    EXPECT_EQ(k.checkMove(X_H, Y_1, flags), false);
    EXPECT_EQ(k.checkMove(X_H, Y_4, flags), false);
}

TEST(KnightTest, Move)
{
    Knight k(PIECE_WHITE, X_D, Y_4);
    int    flags = 0;

    /* Unable to move on raw and columns */
    EXPECT_EQ(k.move(X_H, Y_4, flags), false);
    EXPECT_EQ(k.getX(), X_D);
    EXPECT_EQ(k.getY(), Y_4);

    /* Able to move to diagonals */
    EXPECT_EQ(k.move(X_F, Y_3, flags), true);
    EXPECT_EQ(k.getX(), X_F);
    EXPECT_EQ(k.getY(), Y_3);
}

TEST(KnightTest, GetType)
{
    Knight k(PIECE_WHITE, X_D, Y_4);
    EXPECT_EQ(k.getType(), PieceType::KnightType);
}

TEST(KnightTest, GetValue)
{
    Knight k(PIECE_WHITE, X_D, Y_4);
    EXPECT_EQ(k.getValue(), KNIGHT_VALUE);
}
