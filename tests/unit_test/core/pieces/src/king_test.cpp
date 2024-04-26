/**
 * @file  king_test.cpp
 * @brief King sources unit tests
 */

#include <gtest/gtest.h>
#include "king.hpp"
#include "rook.hpp"

TEST(KingTest, Constructor)
{
    King k1(PIECE_WHITE);
    EXPECT_EQ(k1.getColor(), PIECE_WHITE);
    EXPECT_EQ(k1.getX(), KING_WHITE_DEFAULT_X);
    EXPECT_EQ(k1.getY(), KING_WHITE_DEFAULT_Y);
    EXPECT_EQ(k1.getCheckStatus(), false);
    EXPECT_EQ(k1.getMateStatus(), false);
    EXPECT_EQ(k1.getPatStatus(), false);

    King k2(PIECE_BLACK);
    EXPECT_EQ(k2.getColor(), PIECE_BLACK);
    EXPECT_EQ(k2.getX(), KING_BLACK_DEFAULT_X);
    EXPECT_EQ(k2.getY(), KING_BLACK_DEFAULT_Y);
    EXPECT_EQ(k2.getCheckStatus(), false);
    EXPECT_EQ(k2.getMateStatus(), false);
    EXPECT_EQ(k2.getPatStatus(), false);
}

TEST(KingTest, Castle)
{
    King k(PIECE_WHITE);
    Rook r1(PIECE_WHITE, ROOK_1_WHITE_DEFAULT_X, ROOK_1_WHITE_DEFAULT_Y);
    Rook r2(PIECE_WHITE, ROOK_2_WHITE_DEFAULT_X, ROOK_2_WHITE_DEFAULT_Y);

    /* Unable to castle if is check */
    k.setCheckStatus(true);
    EXPECT_EQ(k.castle(KING_CASTLE, r1), false);

    /* Unable to castle if the given rook isn't the good one */
    k.setCheckStatus(false);
    EXPECT_EQ(k.castle(KING_CASTLE, r1), false);

    /* Success */
    EXPECT_EQ(k.castle(KING_CASTLE, r2), true);
}

TEST(KingTest, CheckMove)
{
    King k(PIECE_WHITE);
    int  flags = 0;

    /* 8| .  .  .  .  .  .  .  .
     * 7| .  .  .  .  .  .  .  .
     * 6| .  .  .  .  .  .  .  .
     * 5| .  .  .  .  .  .  .  .
     * 4| .  .  .  .  .  .  .  .
     * 3| .  .  .  .  .  .  .  .
     * 2| .  .  .  ×  ×  ×  .  .
     * 1| .  .  .  ×  K  x  .  .
     * ___________________________
     *  | a  b  c  d  e  f  g  h 
     */

    /* Able to move to distance of one */
    EXPECT_EQ(k.checkMove(KING_WHITE_DEFAULT_X - 1, KING_WHITE_DEFAULT_Y, flags), true);
    EXPECT_EQ(k.checkMove(KING_WHITE_DEFAULT_X + 1, KING_WHITE_DEFAULT_Y, flags), true);
    EXPECT_EQ(k.checkMove(KING_WHITE_DEFAULT_X - 1, KING_WHITE_DEFAULT_Y + 1, flags), true);
    EXPECT_EQ(k.checkMove(KING_WHITE_DEFAULT_X, KING_WHITE_DEFAULT_Y + 1, flags), true);
    EXPECT_EQ(k.checkMove(KING_WHITE_DEFAULT_X + 1, KING_WHITE_DEFAULT_Y + 1, flags), true);

    /* Unable to move more than one square away - test random value */
    EXPECT_EQ(k.checkMove(X_C, Y_4, flags), false);
    EXPECT_EQ(k.checkMove(X_C, Y_7, flags), false);
    EXPECT_EQ(k.checkMove(X_H, Y_1, flags), false);
    EXPECT_EQ(k.checkMove(X_A, Y_2, flags), false);

    /* King castle */
    flags = 0;
    EXPECT_EQ(k.checkMove(KING_KING_CASTLE_X, KING_WHITE_DEFAULT_Y, flags), true);
    EXPECT_EQ(flags, MOVE_FLAG_KING_CASTLE);

    /* Queen castle */
    flags = 0;
    EXPECT_EQ(k.checkMove(KING_QUEEN_CASTLE_X, KING_WHITE_DEFAULT_Y, flags), true);
    EXPECT_EQ(flags, MOVE_FLAG_QUEEN_CASTLE);
}

TEST(KingTest, Move)
{
    King k(PIECE_WHITE);
    int  flags = 0;

    /* Unable to move more than one square away */
    EXPECT_EQ(k.move(X_C, Y_4, flags), false);
    EXPECT_EQ(k.getX(), KING_WHITE_DEFAULT_X);
    EXPECT_EQ(k.getY(), KING_WHITE_DEFAULT_Y);

    /* Able to move one square away */
    EXPECT_EQ(k.move(KING_WHITE_DEFAULT_X + 1, KING_WHITE_DEFAULT_Y + 1, flags), true);
    EXPECT_EQ(k.getX(), KING_WHITE_DEFAULT_X + 1);
    EXPECT_EQ(k.getY(), KING_WHITE_DEFAULT_Y + 1);
}

TEST(KingTest, getType)
{
    King k(PIECE_WHITE);
    EXPECT_EQ(k.getType(), PieceType::KingType);
}

TEST(KingTest, GetValue)
{
    King k(PIECE_WHITE);
    EXPECT_EQ(k.getValue(), KING_VALUE);
}
