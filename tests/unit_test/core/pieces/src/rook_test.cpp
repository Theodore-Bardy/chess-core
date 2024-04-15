/**
 * @file  rook_test.cpp
 * @brief Rook sources unit tests
 */

#include <gtest/gtest.h>
#include "rook.hpp"

TEST(RookTest, Castling)
{
    Rook r1(true, ROOK_1_WHITE_DEFAULT_X, ROOK_1_WHITE_DEFAULT_Y);
    Rook r2(true, ROOK_2_WHITE_DEFAULT_X, ROOK_2_WHITE_DEFAULT_Y);

    /* Able to castle since it hasn't move */
    EXPECT_EQ(r1.castling(false), true);
    EXPECT_EQ(r1.getX(), ROOK_QUEEN_CASTLE_X);
    EXPECT_EQ(r2.castling(true), true);
    EXPECT_EQ(r2.getX(), ROOK_KING_CASTLE_X);

    // /* Unable to castle since it has move */
    EXPECT_EQ(r1.castling(false), false);
    EXPECT_EQ(r2.castling(true), false);
}

TEST(RookTest, CheckMove)
{
    Rook r(true, 3U, 3U);

    /* Able to move on raw and columns */
    EXPECT_EQ(r.checkMove(3U, 0U, 0U), true);
    EXPECT_EQ(r.checkMove(3U, 7U, 0U), true);
    EXPECT_EQ(r.checkMove(0U, 3U, 0U), true);
    EXPECT_EQ(r.checkMove(7U, 3U, 0U), true);

    /* Unable to move to diagonals */
    EXPECT_EQ(r.checkMove(7U, 7U, 0U), false);
    EXPECT_EQ(r.checkMove(1U, 5U, 0U), false);
    EXPECT_EQ(r.checkMove(1U, 1U, 0U), false);
    EXPECT_EQ(r.checkMove(5U, 1U, 0U), false);
}

TEST(RookTest, Move)
{
    Rook r(true, 3U, 3U);

    /* Unable to move to diagonals */
    EXPECT_EQ(r.move(7U, 7U, 0U), false);
    EXPECT_EQ(r.getX(), 3U);
    EXPECT_EQ(r.getY(), 3U);

    /* Unable to move on raw and columns */
    EXPECT_EQ(r.move(3U, 0U, 0U), true);
    EXPECT_EQ(r.getX(), 3U);
    EXPECT_EQ(r.getY(), 0U);
}

TEST(RookTest, GetValue)
{
    Rook r(true, 3U, 3U);
    EXPECT_EQ(r.getValue(), SquarePieceValue::RookValue);
}
