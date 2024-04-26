/**
 * @file  pawn_test.cpp
 * @brief Pawn sources unit tests
 */

#include <gtest/gtest.h>
#include "pawn.hpp"

TEST(PawnTest, Constructor)
{
    Pawn p1(PIECE_WHITE, X_B);
    EXPECT_EQ(p1.getColor(), PIECE_WHITE);
    EXPECT_EQ(p1.getX(), X_B);
    EXPECT_EQ(p1.getY(), Y_2);

    Pawn p2(PIECE_BLACK, X_B);
    EXPECT_EQ(p2.getColor(), PIECE_BLACK);
    EXPECT_EQ(p2.getX(), X_B);
    EXPECT_EQ(p2.getY(), Y_7);
}

TEST(PawnTest, Promotion)
{
    // TODO
}

TEST(PawnTest, CheckMove)
{
    Pawn p(PIECE_WHITE, X_B);
    int  flags = 0;

    /* The pawn can move right or left only when it takes */
    EXPECT_EQ(p.checkMove(X_A, Y_3, flags), true);
    EXPECT_EQ(p.checkMove(X_C, Y_3, flags), true);

    /* Otherwise the pawn must stay on his column */
    EXPECT_EQ(p.checkMove(X_B, Y_3, flags), true);
    EXPECT_EQ(p.checkMove(X_B, Y_4, flags), true);

    /* Otherwise unable to move - random values */
    EXPECT_EQ(p.checkMove(X_D, Y_1, flags), false);
    EXPECT_EQ(p.checkMove(X_D, Y_8, flags), false);
    EXPECT_EQ(p.checkMove(X_A, Y_4, flags), false);
    EXPECT_EQ(p.checkMove(X_H, Y_4, flags), false);
}

TEST(PawnTest, Move)
{
    Pawn p(PIECE_WHITE, X_B);
    int  flags = 0;

    /* Pawn can move 2 square forward only at the first move */
    EXPECT_EQ(p.move(X_B, Y_4, flags), true);
    EXPECT_EQ(p.getX(), X_B);
    EXPECT_EQ(p.getY(), Y_4);
    EXPECT_EQ(p.move(X_B, Y_6, flags), false);
    EXPECT_EQ(p.getX(), X_B);
    EXPECT_EQ(p.getY(), Y_4);

    /* Then only one square forward */
    EXPECT_EQ(p.move(X_B, Y_5, flags), true);
    EXPECT_EQ(p.getX(), X_B);
    EXPECT_EQ(p.getY(), Y_5);

    /* Or one square forward and one right or left */
    EXPECT_EQ(p.move(X_A, Y_6, flags), true);
    EXPECT_EQ(p.getX(), X_A);
    EXPECT_EQ(p.getY(), Y_6);

    /* Can't move backward */
    EXPECT_EQ(p.move(X_A, Y_5, flags), false);
    EXPECT_EQ(p.getX(), X_A);
    EXPECT_EQ(p.getY(), Y_6);

    /* Otherwise unable to move - random values */
    EXPECT_EQ(p.move(X_D, Y_1, flags), false);
    EXPECT_EQ(p.move(X_D, Y_8, flags), false);
    EXPECT_EQ(p.move(X_A, Y_4, flags), false);
    EXPECT_EQ(p.move(X_H, Y_4, flags), false);
}

TEST(PawnTest, GetType)
{
    Pawn p(PIECE_WHITE, X_B);
    EXPECT_EQ(p.getType(), PieceType::PawnType);
}

TEST(PawnTest, GetValue)
{
    Pawn p(PIECE_WHITE, X_B);
    EXPECT_EQ(p.getValue(), PAWN_VALUE);
}
