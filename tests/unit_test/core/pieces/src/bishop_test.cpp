/**
 * @file  bishop_test.cpp
 * @brief Bishop sources unit tests
 */

#include <gtest/gtest.h>
#include "bishop.hpp"

TEST(BishopTest, Constructor)
{
    Bishop b(PIECE_WHITE, X_D, Y_6);

    EXPECT_EQ(b.getColor(), PIECE_WHITE);
    EXPECT_EQ(b.getX(), X_D);
    EXPECT_EQ(b.getY(), Y_6);
}

TEST(BishopTest, CheckMove)
{
    Bishop b(PIECE_WHITE, X_D, Y_4);
    int    flags = 0;

    /* Able to move to diagonals */
    EXPECT_EQ(b.checkMove(X_H, Y_8, flags), true);
    EXPECT_EQ(b.checkMove(X_B, Y_6, flags), true);
    EXPECT_EQ(b.checkMove(X_B, Y_2, flags), true);
    EXPECT_EQ(b.checkMove(X_F, Y_2, flags), true);

    /* Unable to move on raw and columns */
    EXPECT_EQ(b.checkMove(X_D, Y_1, flags), false);
    EXPECT_EQ(b.checkMove(X_D, Y_8, flags), false);
    EXPECT_EQ(b.checkMove(X_A, Y_4, flags), false);
    EXPECT_EQ(b.checkMove(X_H, Y_4, flags), false);
}

TEST(BishopTest, Move)
{
    Bishop b(PIECE_WHITE, X_D, Y_4);
    int    flags = 0;

    /* Unable to move on raw and columns */
    EXPECT_EQ(b.move(X_D, Y_1, flags), false);
    EXPECT_EQ(b.getX(), X_D);
    EXPECT_EQ(b.getY(), Y_4);

    /* Able to move to diagonals */
    EXPECT_EQ(b.move(X_H, Y_8, flags), true);
    EXPECT_EQ(b.getX(), X_H);
    EXPECT_EQ(b.getY(), Y_8);
}

TEST(BishopTest, GetType)
{
    Bishop b(PIECE_WHITE, X_D, Y_4);
    EXPECT_EQ(b.getType(), PieceType::BishopType);
}

TEST(BishopTest, GetValue)
{
    Bishop b(PIECE_WHITE, X_D, Y_4);
    EXPECT_EQ(b.getValue(), BISHOP_VALUE);
}
