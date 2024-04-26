/**
 * @file  rook_test.cpp
 * @brief Rook sources unit tests
 */

#include <gtest/gtest.h>
#include "rook.hpp"

TEST(RookTest, Constructor)
{
    Rook r1(PIECE_WHITE, ROOK_2_WHITE_DEFAULT_X, ROOK_2_WHITE_DEFAULT_Y);
    EXPECT_EQ(r1.getColor(), PIECE_WHITE);
    EXPECT_EQ(r1.getX(), ROOK_2_WHITE_DEFAULT_X);
    EXPECT_EQ(r1.getY(), ROOK_2_WHITE_DEFAULT_Y);
    EXPECT_EQ(r1.isKingSideRook(), true);

    Rook r2(PIECE_BLACK, X_D, Y_4);
    EXPECT_EQ(r2.getColor(), PIECE_BLACK);
    EXPECT_EQ(r2.getX(), X_D);
    EXPECT_EQ(r2.getY(), Y_4);
    EXPECT_EQ(r2.isKingSideRook(), false);
}

TEST(RookTest, Castle)
{
    /* King side - success */
    Rook r1(PIECE_WHITE, ROOK_2_WHITE_DEFAULT_X, ROOK_2_WHITE_DEFAULT_Y);
    EXPECT_EQ(r1.castle(), true);
    EXPECT_EQ(r1.getX(), X_F);

    /* Queen side - success */
    Rook r2(PIECE_BLACK, ROOK_1_BLACK_DEFAULT_X, ROOK_1_BLACK_DEFAULT_Y);
    EXPECT_EQ(r2.castle(), true);
    EXPECT_EQ(r2.getX(), X_D);

    /* Fail - already moved */
    Rook r3(PIECE_WHITE, ROOK_2_WHITE_DEFAULT_X, ROOK_2_WHITE_DEFAULT_Y);
    int  flags = 0;
    r3.move(X_G, ROOK_2_WHITE_DEFAULT_Y, flags);
    r3.move(ROOK_2_WHITE_DEFAULT_X, ROOK_2_WHITE_DEFAULT_Y, flags);
    EXPECT_EQ(r3.castle(), false);
}

TEST(RookTest, CheckMove)
{
    Rook r(PIECE_BLACK, X_D, Y_4);
    int  flags = 0;

    /* 8| .  .  .  ×  .  .  .  .
     * 7| .  .  .  ×  .  .  .  .
     * 6| .  .  .  ×  .  .  .  .
     * 5| .  .  .  ×  .  .  .  .
     * 4| ×  ×  ×  R  ×  ×  ×  ×
     * 3| .  .  .  ×  .  .  .  .
     * 2| .  .  .  ×  .  .  .  .
     * 1| .  .  .  ×  .  .  .  . 
     * ___________________________
     *  | a  b  c  d  e  f  g  h 
     */

    /* Unable to move on diagonals */
    EXPECT_EQ(r.checkMove(X_E, Y_5, flags), false);
    EXPECT_EQ(r.checkMove(X_C, Y_5, flags), false);
    EXPECT_EQ(r.checkMove(X_H, Y_1, flags), false);
    EXPECT_EQ(r.checkMove(X_B, Y_3, flags), false);

    /* Able to move on raws */
    EXPECT_EQ(r.checkMove(X_D, Y_6, flags), true);
    EXPECT_EQ(r.checkMove(X_A, Y_4, flags), true);
    EXPECT_EQ(r.checkMove(X_G, Y_4, flags), true);
    EXPECT_EQ(r.checkMove(X_D, Y_1, flags), true);
}

TEST(RookTest, Move)
{
    Rook r(PIECE_BLACK, X_D, Y_4);
    int  flags = 0;

    /* Unable to move on diagonals */
    EXPECT_EQ(r.move(X_E, Y_5, flags), false);
    EXPECT_EQ(r.getX(), X_D);
    EXPECT_EQ(r.getY(), Y_4);

    /* Able to move on raws */
    EXPECT_EQ(r.move(X_D, Y_6, flags), true);
    EXPECT_EQ(r.getX(), X_D);
    EXPECT_EQ(r.getY(), Y_6);
    EXPECT_EQ(r.move(X_F, Y_6, flags), true);
    EXPECT_EQ(r.getX(), X_F);
    EXPECT_EQ(r.getY(), Y_6);
    EXPECT_EQ(r.move(X_F, Y_4, flags), true);
    EXPECT_EQ(r.getX(), X_F);
    EXPECT_EQ(r.getY(), Y_4);
    EXPECT_EQ(r.move(X_D, Y_4, flags), true);
    EXPECT_EQ(r.getX(), X_D);
    EXPECT_EQ(r.getY(), Y_4);
}

TEST(RookTest, getType)
{
    Rook r(PIECE_BLACK, X_D, Y_4);
    EXPECT_EQ(r.getType(), PieceType::RookType);
}

TEST(RookTest, GetValue)
{
    Rook r(PIECE_BLACK, X_D, Y_4);
    EXPECT_EQ(r.getValue(), ROOK_VALUE);
}
