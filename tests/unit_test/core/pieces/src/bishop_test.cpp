/**
 * @file  bishop_test.cpp
 * @brief Bishop sources unit tests
 */

#include <gtest/gtest.h>
#include "bishop.hpp"

TEST(BishopTest, IsAbleToMove)
{
    Bishop b(true, 3U, 3U);

    /* Able to move to diagonals */
    EXPECT_EQ(b.isAbleToMove(7U, 7U, 0U), true);
    EXPECT_EQ(b.isAbleToMove(1U, 5U, 0U), true);
    EXPECT_EQ(b.isAbleToMove(1U, 1U, 0U), true);
    EXPECT_EQ(b.isAbleToMove(5U, 1U, 0U), true);

    /* Unable to move on raw and columns */
    EXPECT_EQ(b.isAbleToMove(3U, 0U, 0U), false);
    EXPECT_EQ(b.isAbleToMove(3U, 7U, 0U), false);
    EXPECT_EQ(b.isAbleToMove(0U, 3U, 0U), false);
    EXPECT_EQ(b.isAbleToMove(7U, 3U, 0U), false);
}

TEST(BishopTest, Move)
{
    Bishop b(true, 3U, 3U);

    /* Unable to move on raw and columns */
    EXPECT_EQ(b.move(3U, 0U, 0U), false);
    EXPECT_EQ(b.getX(), 3U);
    EXPECT_EQ(b.getY(), 3U);

    /* Able to move to diagonals */
    EXPECT_EQ(b.move(7U, 7U, 0U), true);
    EXPECT_EQ(b.getX(), 7U);
    EXPECT_EQ(b.getY(), 7U);
}

TEST(BishopTest, GetValue)
{
    Bishop b(true, 3U, 3U);
    EXPECT_EQ(b.getValue(), SquarePieceValue::BishopValue);
}
