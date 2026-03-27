/**
 * @file  board.cpp
 * @brief Board sources
 */

#include <assert.h>
#include <cstdlib>
#include <string>
#include "board.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "bishop.hpp"
#include "knight.hpp"
#include "rook.hpp"
#include "pawn.hpp"
#include "move.hpp"

Board::Board()
    : isInitialize(false)
    , enPassantX(-1)
    , enPassantY(-1)
    , halfMoveClock(0)
{
    /* Initialize empty board */
    for (int Y = 0; Y < BOARD_SIZE_MAX; Y++)
    {
        for (int X = 0; X < BOARD_SIZE_MAX; X++)
        {
            this->board[X][Y] = new Square(X, Y);
        }
    }
}

Board::Board(Board const& boardToCopy)
    : isInitialize(boardToCopy.isInitialize)
    , enPassantX(boardToCopy.enPassantX)
    , enPassantY(boardToCopy.enPassantY)
    , halfMoveClock(boardToCopy.halfMoveClock)
    , positionHistory(boardToCopy.positionHistory)
{
    /* Copy squares */
    for (int Y = 0; Y < BOARD_SIZE_MAX; Y++)
    {
        for (int X = 0; X < BOARD_SIZE_MAX; X++)
        {
            board[X][Y] = new Square(*boardToCopy.board[X][Y]);
        }
    }

    /* White pieces */
    for (auto const& it : boardToCopy.whitePieces)
    {
        whitePieces.push_back(it->clone());
    }

    /* Black pieces */
    for (auto const& it : boardToCopy.blackPieces)
    {
        blackPieces.push_back(it->clone());
    }

    /* Repair named pointers that now point into the copied vectors */
    setupNamedPointers();
}

void
Board::setupNamedPointers()
{
    int wBishop = 0, wKnight = 0, wRook = 0, wPawn = 0;
    int bBishop = 0, bKnight = 0, bRook = 0, bPawn = 0;

    for (Piece* p : whitePieces)
    {
        switch (p->getValue())
        {
            case SquarePieceValue::KingValue:   whiteKing           = static_cast<King*>(p);           break;
            case SquarePieceValue::QueenValue:  whiteQueen          = static_cast<Queen*>(p);          break;
            case SquarePieceValue::BishopValue: whiteBishops[wBishop++] = static_cast<Bishop*>(p);    break;
            case SquarePieceValue::KnightValue: whiteKnights[wKnight++] = static_cast<Knight*>(p);    break;
            case SquarePieceValue::RookValue:   whiteRooks[wRook++]     = static_cast<Rook*>(p);      break;
            case SquarePieceValue::PawnValue:   whitePawns[wPawn++]     = static_cast<Pawn*>(p);      break;
            default: break;
        }
    }

    for (Piece* p : blackPieces)
    {
        switch (p->getValue())
        {
            case SquarePieceValue::KingValue:   blackKing           = static_cast<King*>(p);           break;
            case SquarePieceValue::QueenValue:  blackQueen          = static_cast<Queen*>(p);          break;
            case SquarePieceValue::BishopValue: blackBishops[bBishop++] = static_cast<Bishop*>(p);    break;
            case SquarePieceValue::KnightValue: blackKnights[bKnight++] = static_cast<Knight*>(p);    break;
            case SquarePieceValue::RookValue:   blackRooks[bRook++]     = static_cast<Rook*>(p);      break;
            case SquarePieceValue::PawnValue:   blackPawns[bPawn++]     = static_cast<Pawn*>(p);      break;
            default: break;
        }
    }
}

Board::~Board()
{
    /* Release board */
    for (int Y = 0; Y < BOARD_SIZE_MAX; Y++)
    {
        for (int X = 0; X < BOARD_SIZE_MAX; X++)
        {
            delete board[X][Y];
        }
    }

    /* Release piece vectors */
    for (Piece* p : whitePieces)
    {
        delete p;
    }

    for (Piece* p : blackPieces)
    {
        delete p;
    }
}

void
Board::startUp()
{
    /* Check the startUp function is called once */
    if (!isInitialize)
    {
        isInitialize = true;

        /* White pieces */
        King* white_king = new King(PIECE_WHITE);
        board[KING_WHITE_DEFAULT_X][KING_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::KingValue);
        board[KING_WHITE_DEFAULT_X][KING_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_king);
        whiteKing = white_king;

        Queen* white_queen = new Queen(PIECE_WHITE);
        board[QUEEN_WHITE_DEFAULT_X][QUEEN_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::QueenValue);
        board[QUEEN_WHITE_DEFAULT_X][QUEEN_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_queen);
        whiteQueen = white_queen;

        Bishop* white_bishop = new Bishop(PIECE_WHITE, BISHOP_1_WHITE_DEFAULT_X, BISHOP_1_WHITE_DEFAULT_Y);
        board[BISHOP_1_WHITE_DEFAULT_X][BISHOP_1_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::BishopValue);
        board[BISHOP_1_WHITE_DEFAULT_X][BISHOP_1_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_bishop);
        whiteBishops[0] = white_bishop;

        white_bishop = new Bishop(PIECE_WHITE, BISHOP_2_WHITE_DEFAULT_X, BISHOP_2_WHITE_DEFAULT_Y);
        board[BISHOP_2_WHITE_DEFAULT_X][BISHOP_2_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::BishopValue);
        board[BISHOP_2_WHITE_DEFAULT_X][BISHOP_2_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_bishop);
        whiteBishops[1] = white_bishop;

        Knight* white_knight = new Knight(PIECE_WHITE, KNIGHT_1_WHITE_DEFAULT_X, KNIGHT_1_WHITE_DEFAULT_Y);
        board[KNIGHT_1_WHITE_DEFAULT_X][KNIGHT_1_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::KnightValue);
        board[KNIGHT_1_WHITE_DEFAULT_X][KNIGHT_1_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_knight);
        whiteKnights[0] = white_knight;

        white_knight = new Knight(PIECE_WHITE, KNIGHT_2_WHITE_DEFAULT_X, KNIGHT_2_WHITE_DEFAULT_Y);
        board[KNIGHT_2_WHITE_DEFAULT_X][KNIGHT_2_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::KnightValue);
        board[KNIGHT_2_WHITE_DEFAULT_X][KNIGHT_2_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_knight);
        whiteKnights[1] = white_knight;

        Rook* white_rook = new Rook(PIECE_WHITE, ROOK_1_WHITE_DEFAULT_X, ROOK_1_WHITE_DEFAULT_Y);
        board[ROOK_1_WHITE_DEFAULT_X][ROOK_1_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::RookValue);
        board[ROOK_1_WHITE_DEFAULT_X][ROOK_1_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_rook);
        whiteRooks[0] = white_rook;

        white_rook = new Rook(PIECE_WHITE, ROOK_2_WHITE_DEFAULT_X, ROOK_2_WHITE_DEFAULT_Y);
        board[ROOK_2_WHITE_DEFAULT_X][ROOK_2_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::RookValue);
        board[ROOK_2_WHITE_DEFAULT_X][ROOK_2_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_rook);
        whiteRooks[1] = white_rook;

        /* Black pieces */
        King* black_king = new King(PIECE_BLACK);
        board[KING_BLACK_DEFAULT_X][KING_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::KingValue);
        board[KING_BLACK_DEFAULT_X][KING_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_king);
        blackKing = black_king;

        Queen* black_queen = new Queen(PIECE_BLACK);
        board[QUEEN_BLACK_DEFAULT_X][QUEEN_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::QueenValue);
        board[QUEEN_BLACK_DEFAULT_X][QUEEN_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_queen);
        blackQueen = black_queen;

        Bishop* black_bishop = new Bishop(PIECE_BLACK, BISHOP_1_BLACK_DEFAULT_X, BISHOP_1_BLACK_DEFAULT_Y);
        board[BISHOP_1_BLACK_DEFAULT_X][BISHOP_1_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::BishopValue);
        board[BISHOP_1_BLACK_DEFAULT_X][BISHOP_1_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_bishop);
        blackBishops[0] = black_bishop;

        black_bishop = new Bishop(PIECE_BLACK, BISHOP_2_BLACK_DEFAULT_X, BISHOP_2_BLACK_DEFAULT_Y);
        board[BISHOP_2_BLACK_DEFAULT_X][BISHOP_2_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::BishopValue);
        board[BISHOP_2_BLACK_DEFAULT_X][BISHOP_2_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_bishop);
        blackBishops[1] = black_bishop;

        Knight* black_knight = new Knight(PIECE_BLACK, KNIGHT_1_BLACK_DEFAULT_X, KNIGHT_1_BLACK_DEFAULT_Y);
        board[KNIGHT_1_BLACK_DEFAULT_X][KNIGHT_1_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::KnightValue);
        board[KNIGHT_1_BLACK_DEFAULT_X][KNIGHT_1_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_knight);
        blackKnights[0] = black_knight;

        black_knight = new Knight(PIECE_BLACK, KNIGHT_2_BLACK_DEFAULT_X, KNIGHT_2_BLACK_DEFAULT_Y);
        board[KNIGHT_2_BLACK_DEFAULT_X][KNIGHT_2_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::KnightValue);
        board[KNIGHT_2_BLACK_DEFAULT_X][KNIGHT_2_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_knight);
        blackKnights[1] = black_knight;

        Rook* black_rook = new Rook(PIECE_BLACK, ROOK_1_BLACK_DEFAULT_X, ROOK_1_BLACK_DEFAULT_Y);
        board[ROOK_1_BLACK_DEFAULT_X][ROOK_1_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::RookValue);
        board[ROOK_1_BLACK_DEFAULT_X][ROOK_1_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_rook);
        blackRooks[0] = black_rook;

        black_rook = new Rook(PIECE_BLACK, ROOK_2_BLACK_DEFAULT_X, ROOK_2_BLACK_DEFAULT_Y);
        board[ROOK_2_BLACK_DEFAULT_X][ROOK_2_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::RookValue);
        board[ROOK_2_BLACK_DEFAULT_X][ROOK_2_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_rook);
        blackRooks[1] = black_rook;

        /* Pawns */
        for (int index = 0; index < BOARD_SIZE_MAX; index++)
        {
            /* White pawns */
            Pawn* white_pawn = new Pawn(PIECE_WHITE, index);
            board[index][PAWN_WHITE_DEFAULT_Y]->setValue(SquarePieceValue::PawnValue);
            board[index][PAWN_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
            whitePieces.push_back(white_pawn);
            whitePawns[index] = white_pawn;

            /* Black pawns */
            Pawn* black_pawn = new Pawn(PIECE_BLACK, index);
            board[index][PAWN_BLACK_DEFAULT_Y]->setValue(SquarePieceValue::PawnValue);
            board[index][PAWN_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
            blackPieces.push_back(black_pawn);
            blackPawns[index] = black_pawn;
        }
    }
}

bool
Board::movePiece(Piece* piece, int x, int y, int flags, SquarePieceValue promotionPiece)
{
    /* Check parameter */
    assert(nullptr != piece);

    bool xReturn = false;
    int  oldX    = piece->getX();
    int  oldY    = piece->getY();

    /* En passant capture: the captured pawn is beside the moving pawn, not at the destination */
    if ((flags & MOVE_FLAG_EN_PASSANT) && (flags & MOVE_FLAG_TAKE))
    {
        int capturedY = oldY; /* Captured pawn sits on the same rank as the moving pawn */
        Piece* captured = nullptr;
        if (this->selectPiece(&captured, x, capturedY))
        {
            piece->eat(*captured);
            board[x][capturedY]->resetSquare();
        }
        /* Destination is empty — the normal move branch will place the pawn there */
    }
    /* Regular capture */
    else if ((MOVE_FLAG_TAKE == (flags & MOVE_FLAG_TAKE)))
    {
        Piece* p = nullptr;
        if (this->selectPiece(&p, x, y))
        {
            piece->eat(*p);

            /* Reset square */
            board[x][y]->resetSquare();
        }
    }

    /* Special move - king castle */
    if ((MOVE_FLAG_KING_CASTLE == (flags & MOVE_FLAG_KING_CASTLE)))
    {
        Rook* r = nullptr;
        King* k = (piece->getColor()) ? whiteKing : blackKing;
        assert(k == piece);

        /* Find king side rook */
        if (piece->getColor())
        {
            r = (whiteRooks[0]->isKingSideRook()) ? whiteRooks[0] : whiteRooks[1];
        }
        else
        {
            r = (blackRooks[0]->isKingSideRook()) ? blackRooks[0] : blackRooks[1];
        }

        /* Check king and rook can castle, king is not in check, and king does not pass through or land on attacked square */
        if (!r->getHasMoved() && k->isAbleToMove(KING_KING_CASTLE_X, y, flags, board) && r->isAbleToMove(ROOK_KING_CASTLE_X, y, flags, board)
            && !this->isInCheck(piece->getColor())
            && !this->isSquareAttacked(!piece->getColor(), k->getX() + 1, y) && !this->isSquareAttacked(!piece->getColor(), k->getX() + 2, y))
        {
            /* Reset king and rook squares */
            board[k->getX()][k->getY()]->resetSquare();
            board[r->getX()][r->getY()]->resetSquare();

            /* Move king and rook */
            k->castling(true);
            r->castling(true);
            if (piece->getColor())
            {
                board[k->getX()][k->getY()]->setColor(SquarePieceColor::White);
                board[r->getX()][r->getY()]->setColor(SquarePieceColor::White);
            }
            else
            {
                board[k->getX()][k->getY()]->setColor(SquarePieceColor::Black);
                board[r->getX()][r->getY()]->setColor(SquarePieceColor::Black);
            }
            board[k->getX()][k->getY()]->setValue(SquarePieceValue::KingValue);
            board[r->getX()][r->getY()]->setValue(SquarePieceValue::RookValue);

            /* Castling succeeds: update draw tracking and return success */
            enPassantX = -1;
            enPassantY = -1;
            updateDrawTracking(piece->getColor(), false, false);
            xReturn = true;
        }
    }

    /* Special move - queen castle */
    else if ((MOVE_FLAG_QUEEN_CASTLE == (flags & MOVE_FLAG_QUEEN_CASTLE)))
    {
        Rook* r = nullptr;
        King* k = (piece->getColor()) ? whiteKing : blackKing;
        assert(k == piece);

        /* Find king side rook */
        if (piece->getColor())
        {
            r = (!whiteRooks[0]->isKingSideRook()) ? whiteRooks[0] : whiteRooks[1];
        }
        else
        {
            r = (!blackRooks[0]->isKingSideRook()) ? blackRooks[0] : blackRooks[1];
        }

        /* Check king and rook can castle, king is not in check, and king does not pass through or land on attacked square */
        if (!r->getHasMoved() && k->isAbleToMove(KING_QUEEN_CASTLE_X, y, flags, board) && r->isAbleToMove(ROOK_QUEEN_CASTLE_X, y, flags, board)
            && !this->isInCheck(piece->getColor())
            && !this->isSquareAttacked(!piece->getColor(), k->getX() - 1, y) && !this->isSquareAttacked(!piece->getColor(), k->getX() - 2, y))
        {
            /* Reset king and rook squares */
            board[k->getX()][k->getY()]->resetSquare();
            board[r->getX()][r->getY()]->resetSquare();

            /* Move king and rook */
            k->castling(false);
            r->castling(false);
            if (piece->getColor())
            {
                board[k->getX()][k->getY()]->setColor(SquarePieceColor::White);
                board[r->getX()][r->getY()]->setColor(SquarePieceColor::White);
            }
            else
            {
                board[k->getX()][k->getY()]->setColor(SquarePieceColor::Black);
                board[r->getX()][r->getY()]->setColor(SquarePieceColor::Black);
            }
            board[k->getX()][k->getY()]->setValue(SquarePieceValue::KingValue);
            board[r->getX()][r->getY()]->setValue(SquarePieceValue::RookValue);

            /* Castling succeeds: update draw tracking and return success */
            enPassantX = -1;
            enPassantY = -1;
            updateDrawTracking(piece->getColor(), false, false);
            xReturn = true;
        }
    }

    else if (piece->isAbleToMove(x, y, flags, board) && piece->move(x, y, flags))
    {
        /* Reset old square */
        board[oldX][oldY]->setValue(SquarePieceValue::Empty);
        board[oldX][oldY]->setColor(SquarePieceColor::NoPiece);

        /* Fill new square */
        if (piece->getColor())
        {
            board[x][y]->setColor(SquarePieceColor::White);
        }
        else
        {
            board[x][y]->setColor(SquarePieceColor::Black);
        }

        board[x][y]->setValue(piece->getValue());

        /* Pawn promotion: replace the pawn with the chosen piece */
        if ((flags & MOVE_FLAG_PROMOTION) && piece->getValue() == SquarePieceValue::PawnValue)
        {
            piece->eat(*piece); /* Mark the pawn as dead */

            Piece* promoted = nullptr;
            bool pawnColor  = piece->getColor();
            switch (promotionPiece)
            {
                case SquarePieceValue::QueenValue:
                    promoted = new Queen(true, pawnColor, x, y);
                    break;
                case SquarePieceValue::RookValue:
                    promoted = new Rook(false, true, pawnColor, x, y);
                    break;
                case SquarePieceValue::BishopValue:
                    promoted = new Bishop(true, pawnColor, x, y);
                    break;
                case SquarePieceValue::KnightValue:
                    promoted = new Knight(true, pawnColor, x, y);
                    break;
                default:
                    /* Default to queen */
                    promoted = new Queen(true, pawnColor, x, y);
                    break;
            }

            if (pawnColor)
            {
                whitePieces.push_back(promoted);
            }
            else
            {
                blackPieces.push_back(promoted);
            }

            board[x][y]->setValue(promoted->getValue());
        }

        /* Set or clear the en passant target after a pawn double-push */
        if (piece->getValue() == SquarePieceValue::PawnValue && abs(y - oldY) == 2)
        {
            /* The en passant target square is the square the pawn skipped */
            enPassantX = x;
            enPassantY = (oldY + y) / 2;
        }
        else
        {
            enPassantX = -1;
            enPassantY = -1;
        }

        /* Update draw tracking (fifty-move clock + position history) */
        bool isCapture  = (flags & MOVE_FLAG_TAKE) != 0;
        bool isPawnMove = (piece->getValue() == SquarePieceValue::PawnValue);
        updateDrawTracking(piece->getColor(), isCapture, isPawnMove);

        xReturn = true;
    }
    else
    {
        /* Castling and failed moves clear the en passant target */
        enPassantX = -1;
        enPassantY = -1;
    }

    return xReturn;
}

bool
Board::selectPiece(Piece** piece, int x, int y)
{
    if ((board[x][y]->getColor() != SquarePieceColor::NoPiece) && (board[x][y]->getValue() != SquarePieceValue::Empty))
    {
        if (board[x][y]->getColor() == SquarePieceColor::White)
        {
            switch (board[x][y]->getValue())
            {
                case SquarePieceValue::KingValue:
                    *piece = whiteKing;
                    break;

                case SquarePieceValue::QueenValue:
                    *piece = whiteQueen;
                    break;

                case SquarePieceValue::BishopValue:
                    if ((whiteBishops[0]->getX() == x) && (whiteBishops[0]->getY() == y))
                    {
                        *piece = whiteBishops[0];
                    }
                    else
                    {
                        *piece = whiteBishops[1];
                    }
                    break;

                case SquarePieceValue::KnightValue:
                    if ((whiteKnights[0]->getX() == x) && (whiteKnights[0]->getY() == y))
                    {
                        *piece = whiteKnights[0];
                    }
                    else
                    {
                        *piece = whiteKnights[1];
                    }
                    break;

                case SquarePieceValue::RookValue:
                    if ((whiteRooks[0]->getX() == x) && (whiteRooks[0]->getY() == y))
                    {
                        *piece = whiteRooks[0];
                    }
                    else
                    {
                        *piece = whiteRooks[1];
                    }
                    break;

                case SquarePieceValue::PawnValue:
                    if ((whitePawns[0]->getX() == x) && (whitePawns[0]->getY() == y))
                    {
                        *piece = whitePawns[0];
                    }
                    else if ((whitePawns[1]->getX() == x) && (whitePawns[1]->getY() == y))
                    {
                        *piece = whitePawns[1];
                    }
                    else if ((whitePawns[2]->getX() == x) && (whitePawns[2]->getY() == y))
                    {
                        *piece = whitePawns[2];
                    }
                    else if ((whitePawns[3]->getX() == x) && (whitePawns[3]->getY() == y))
                    {
                        *piece = whitePawns[3];
                    }
                    else if ((whitePawns[4]->getX() == x) && (whitePawns[4]->getY() == y))
                    {
                        *piece = whitePawns[4];
                    }
                    else if ((whitePawns[5]->getX() == x) && (whitePawns[5]->getY() == y))
                    {
                        *piece = whitePawns[5];
                    }
                    else if ((whitePawns[6]->getX() == x) && (whitePawns[6]->getY() == y))
                    {
                        *piece = whitePawns[6];
                    }
                    else
                    {
                        *piece = whitePawns[7];
                    }
                    break;

                default:
                    return false;
            }
        }
        else
        {
            switch (board[x][y]->getValue())
            {
                case SquarePieceValue::KingValue:
                    *piece = blackKing;
                    break;

                case SquarePieceValue::QueenValue:
                    *piece = blackQueen;
                    break;

                case SquarePieceValue::BishopValue:
                    if ((blackBishops[0]->getX() == x) && (blackBishops[0]->getY() == y))
                    {
                        *piece = blackBishops[0];
                    }
                    else
                    {
                        *piece = blackBishops[1];
                    }
                    break;

                case SquarePieceValue::KnightValue:
                    if ((blackKnights[0]->getX() == x) && (blackKnights[0]->getY() == y))
                    {
                        *piece = blackKnights[0];
                    }
                    else
                    {
                        *piece = blackKnights[1];
                    }
                    break;

                case SquarePieceValue::RookValue:
                    if ((blackRooks[0]->getX() == x) && (blackRooks[0]->getY() == y))
                    {
                        *piece = blackRooks[0];
                    }
                    else
                    {
                        *piece = blackRooks[1];
                    }
                    break;

                case SquarePieceValue::PawnValue:
                    if ((blackPawns[0]->getX() == x) && (blackPawns[0]->getY() == y))
                    {
                        *piece = blackPawns[0];
                    }
                    else if ((blackPawns[1]->getX() == x) && (blackPawns[1]->getY() == y))
                    {
                        *piece = blackPawns[1];
                    }
                    else if ((blackPawns[2]->getX() == x) && (blackPawns[2]->getY() == y))
                    {
                        *piece = blackPawns[2];
                    }
                    else if ((blackPawns[3]->getX() == x) && (blackPawns[3]->getY() == y))
                    {
                        *piece = blackPawns[3];
                    }
                    else if ((blackPawns[4]->getX() == x) && (blackPawns[4]->getY() == y))
                    {
                        *piece = blackPawns[4];
                    }
                    else if ((blackPawns[5]->getX() == x) && (blackPawns[5]->getY() == y))
                    {
                        *piece = blackPawns[5];
                    }
                    else if ((blackPawns[6]->getX() == x) && (blackPawns[6]->getY() == y))
                    {
                        *piece = blackPawns[6];
                    }
                    else
                    {
                        *piece = blackPawns[7];
                    }
                    break;

                default:
                    return false;
            }
        }
        return true;
    }

    return false;
}

bool
Board::checkMove(Piece** piece, SquarePieceValue pieceType, SquarePieceColor pieceColor, int x, int y, int flags, char extraFlag)
{
    /* Check parameter */
    assert(nullptr == *piece);

    int xExtraFlag = -1;
    int yExtraFlag = -1;

    /* Check for extra flag */
    switch (extraFlag)
    {
        case 'a':
            xExtraFlag = 0;
            break;
        case 'b':
            xExtraFlag = 1;
            break;
        case 'c':
            xExtraFlag = 2;
            break;
        case 'd':
            xExtraFlag = 3;
            break;
        case 'e':
            xExtraFlag = 4;
            break;
        case 'f':
            xExtraFlag = 5;
            break;
        case 'g':
            xExtraFlag = 6;
            break;
        case 'h':
            xExtraFlag = 7;
            break;
        case '1':
            yExtraFlag = 0;
            break;
        case '2':
            yExtraFlag = 1;
            break;
        case '3':
            yExtraFlag = 2;
            break;
        case '4':
            yExtraFlag = 3;
            break;
        case '5':
            yExtraFlag = 4;
            break;
        case '6':
            yExtraFlag = 5;
            break;
        case '7':
            yExtraFlag = 6;
            break;
        case '8':
            yExtraFlag = 7;
            break;
        default:
            /* Nothing to do */
            break;
    }

    /* Lambda declaration - search a particular piece that can legally make the move */
    auto searchPiece = [&piece, &pieceType, &x, &y, &flags, &xExtraFlag, &yExtraFlag, this](vector<Piece*> const& pieces) {
        for (auto p : pieces)
        {
            if ((p->getValue() == pieceType) && p->isAlive() && p->isAbleToMove(x, y, flags) && ((xExtraFlag == -1) || (p->getX() == xExtraFlag))
                && ((yExtraFlag == -1) || (p->getY() == yExtraFlag)) && this->isMoveLegal(p, x, y, flags))
            {
                /* Only one piece should fit to the description */
                assert(nullptr == *piece);
                *piece = p;
            }
        }
        return (*piece != nullptr);
    };

    return searchPiece((pieceColor == White) ? whitePieces : blackPieces);
}

bool
Board::isSquareAttacked(bool pieceColor, int x, int y)
{
    /* MOVE_FLAG_TAKE is required so pawns check diagonal attack squares, not forward movement */
    auto isAttacked = [&x, &y](vector<Piece*> const& pieces, Square* board[BOARD_SIZE_MAX][BOARD_SIZE_MAX]) {
        for (auto p : pieces)
        {
            if (p->isAlive() && p->isAbleToMove(x, y, MOVE_FLAG_TAKE, board))
            {
                return true;
            }
        }
        return false;
    };

    return isAttacked(((pieceColor) ? whitePieces : blackPieces), board);
}

bool
Board::isInCheck(bool color)
{
    /* Find the king of the given color by scanning the piece vector */
    vector<Piece*>& myPieces = color ? whitePieces : blackPieces;
    for (Piece* p : myPieces)
    {
        if (p->isAlive() && p->getValue() == SquarePieceValue::KingValue)
        {
            /* Check if any opponent piece attacks the king's square */
            return isSquareAttacked(!color, p->getX(), p->getY());
        }
    }
    return false;
}

bool
Board::isMoveLegal(Piece* piece, int xEnd, int yEnd, int flags)
{
    /* Apply the move on a temporary copy and check that the own king is not in check */
    Board tempBoard(*this);

    Piece* tempPiece = nullptr;
    tempBoard.selectPiece(&tempPiece, piece->getX(), piece->getY());
    if (!tempPiece)
    {
        return false;
    }

    /* For castling, movePiece validates all conditions inline (check, intermediate squares) */
    bool moveApplied = tempBoard.movePiece(tempPiece, xEnd, yEnd, flags);
    if (!moveApplied)
    {
        return false;
    }

    return !tempBoard.isInCheck(piece->getColor());
}

std::vector<Move>
Board::getLegalMoves(bool color)
{
    std::vector<Move> legalMoves;

    SquarePieceColor ownColor = color ? SquarePieceColor::White : SquarePieceColor::Black;
    std::vector<Piece*>& myPieces = color ? whitePieces : blackPieces;

    for (Piece* p : myPieces)
    {
        if (!p->isAlive())
        {
            continue;
        }

        for (int destX = 0; destX < BOARD_SIZE_MAX; destX++)
        {
            for (int destY = 0; destY < BOARD_SIZE_MAX; destY++)
            {
                /* Skip destination occupied by own piece */
                if (board[destX][destY]->getColor() == ownColor)
                {
                    continue;
                }

                /* Determine capture flag */
                int flags = (board[destX][destY]->getValue() != SquarePieceValue::Empty) ? MOVE_FLAG_TAKE : 0;

                /* Check piece movement rules with full board context */
                if (!p->isAbleToMove(destX, destY, flags, board))
                {
                    continue;
                }

                /* Check that the move does not leave own king in check */
                if (!isMoveLegal(p, destX, destY, flags))
                {
                    continue;
                }

                /* Pawn promotion: reaching the back rank generates 4 moves (one per piece type) */
                bool isPromotion = (p->getValue() == SquarePieceValue::PawnValue)
                                   && ((color && destY == SQUARE_Y_8) || (!color && destY == SQUARE_Y_1));

                if (isPromotion)
                {
                    int promoFlags = flags | MOVE_FLAG_PROMOTION;
                    legalMoves.push_back(Move(p, p->getX(), p->getY(), destX, destY, promoFlags, ' ', SquarePieceValue::QueenValue));
                    legalMoves.push_back(Move(p, p->getX(), p->getY(), destX, destY, promoFlags, ' ', SquarePieceValue::RookValue));
                    legalMoves.push_back(Move(p, p->getX(), p->getY(), destX, destY, promoFlags, ' ', SquarePieceValue::BishopValue));
                    legalMoves.push_back(Move(p, p->getX(), p->getY(), destX, destY, promoFlags, ' ', SquarePieceValue::KnightValue));
                }
                else
                {
                    legalMoves.push_back(Move(p, p->getX(), p->getY(), destX, destY, flags));
                }
            }
        }

        /* En passant: check if this pawn can capture en passant */
        if (p->getValue() == SquarePieceValue::PawnValue && enPassantX != -1)
        {
            int epFlags = MOVE_FLAG_EN_PASSANT | MOVE_FLAG_TAKE;
            if (p->isAbleToMove(enPassantX, enPassantY, epFlags, board) && isMoveLegal(p, enPassantX, enPassantY, epFlags))
            {
                legalMoves.push_back(Move(p, p->getX(), p->getY(), enPassantX, enPassantY, epFlags));
            }
        }

        /* Castling: try both sides for the king */
        if (p->getValue() == SquarePieceValue::KingValue)
        {
            King* k = static_cast<King*>(p);
            if (!k->getHasMoved())
            {
                int castleY = p->getY();
                if (isMoveLegal(p, KING_KING_CASTLE_X, castleY, MOVE_FLAG_KING_CASTLE))
                {
                    legalMoves.push_back(Move(p, p->getX(), castleY, KING_KING_CASTLE_X, castleY, MOVE_FLAG_KING_CASTLE));
                }
                if (isMoveLegal(p, KING_QUEEN_CASTLE_X, castleY, MOVE_FLAG_QUEEN_CASTLE))
                {
                    legalMoves.push_back(Move(p, p->getX(), castleY, KING_QUEEN_CASTLE_X, castleY, MOVE_FLAG_QUEEN_CASTLE));
                }
            }
        }
    }

    return legalMoves;
}

std::string
Board::getFEN(bool colorToMove, int fullMoveNumber) const
{
    std::string fen;

    /* 1. Piece placement: rank 8 (Y=7) down to rank 1 (Y=0) */
    for (int y = BOARD_SIZE_MAX - 1; y >= 0; y--)
    {
        int empty = 0;
        for (int x = 0; x < BOARD_SIZE_MAX; x++)
        {
            SquarePieceValue val = board[x][y]->getValue();
            if (val == SquarePieceValue::Empty)
            {
                empty++;
            }
            else
            {
                if (empty > 0)
                {
                    fen += (char)('0' + empty);
                    empty = 0;
                }
                bool isWhite = (board[x][y]->getColor() == SquarePieceColor::White);
                switch (val)
                {
                    case SquarePieceValue::KingValue:   fen += isWhite ? 'K' : 'k'; break;
                    case SquarePieceValue::QueenValue:  fen += isWhite ? 'Q' : 'q'; break;
                    case SquarePieceValue::RookValue:   fen += isWhite ? 'R' : 'r'; break;
                    case SquarePieceValue::BishopValue: fen += isWhite ? 'B' : 'b'; break;
                    case SquarePieceValue::KnightValue: fen += isWhite ? 'N' : 'n'; break;
                    case SquarePieceValue::PawnValue:   fen += isWhite ? 'P' : 'p'; break;
                    default: break;
                }
            }
        }
        if (empty > 0)
        {
            fen += (char)('0' + empty);
        }
        if (y > 0)
        {
            fen += '/';
        }
    }

    /* 2. Active color */
    fen += colorToMove ? " w " : " b ";

    /* 3. Castling rights */
    Rook* wKsRook = (whiteRooks[0]->isKingSideRook()) ? whiteRooks[0] : whiteRooks[1];
    Rook* wQsRook = (!whiteRooks[0]->isKingSideRook()) ? whiteRooks[0] : whiteRooks[1];
    Rook* bKsRook = (blackRooks[0]->isKingSideRook()) ? blackRooks[0] : blackRooks[1];
    Rook* bQsRook = (!blackRooks[0]->isKingSideRook()) ? blackRooks[0] : blackRooks[1];

    std::string castling;
    if (whiteKing->isAlive() && !whiteKing->getHasMoved())
    {
        if (wKsRook->isAlive() && !wKsRook->getHasMoved()) castling += 'K';
        if (wQsRook->isAlive() && !wQsRook->getHasMoved()) castling += 'Q';
    }
    if (blackKing->isAlive() && !blackKing->getHasMoved())
    {
        if (bKsRook->isAlive() && !bKsRook->getHasMoved()) castling += 'k';
        if (bQsRook->isAlive() && !bQsRook->getHasMoved()) castling += 'q';
    }
    fen += (castling.empty() ? "-" : castling) + " ";

    /* 4. En passant target square */
    if (enPassantX == -1)
    {
        fen += "- ";
    }
    else
    {
        fen += (char)('a' + enPassantX);
        fen += (char)('1' + enPassantY);
        fen += ' ';
    }

    /* 5. Halfmove clock */
    fen += std::to_string(halfMoveClock) + " ";

    /* 6. Fullmove number */
    fen += std::to_string(fullMoveNumber);

    return fen;
}

std::string
Board::computeHash(bool colorToMove) const
{
    std::string hash;
    hash.reserve(128);

    /* Encode each square: color character + value digit */
    for (int Y = 0; Y < BOARD_SIZE_MAX; Y++)
    {
        for (int X = 0; X < BOARD_SIZE_MAX; X++)
        {
            char c = '.';
            if (board[X][Y]->getValue() != SquarePieceValue::Empty)
            {
                bool isWhite = (board[X][Y]->getColor() == SquarePieceColor::White);
                switch (board[X][Y]->getValue())
                {
                    case SquarePieceValue::KingValue:   c = isWhite ? 'K' : 'k'; break;
                    case SquarePieceValue::QueenValue:  c = isWhite ? 'Q' : 'q'; break;
                    case SquarePieceValue::RookValue:   c = isWhite ? 'R' : 'r'; break;
                    case SquarePieceValue::BishopValue: c = isWhite ? 'B' : 'b'; break;
                    case SquarePieceValue::KnightValue: c = isWhite ? 'N' : 'n'; break;
                    case SquarePieceValue::PawnValue:   c = isWhite ? 'P' : 'p'; break;
                    default: break;
                }
            }
            hash += c;
        }
    }

    /* Encode side to move */
    hash += colorToMove ? 'w' : 'b';

    /* Encode en passant file (-1 = none, 0-7 = a-h) */
    hash += (char)('0' + (enPassantX + 1)); /* +1 so -1 becomes '0', 0 becomes '1', etc. */

    /* Castling rights: check hasMoved flags via the named pointers */
    /* These fields are protected; we access them through the known initial positions */
    /* For a proper hash, castling rights would be encoded here — simplified for Phase 1 */

    return hash;
}

void
Board::updateDrawTracking(bool colorToMove, bool isCapture, bool isPawnMove)
{
    /* Fifty-move clock: reset on capture or pawn move, otherwise increment */
    if (isCapture || isPawnMove)
    {
        halfMoveClock = 0;
    }
    else
    {
        halfMoveClock++;
    }

    /* Threefold repetition: record the current position */
    std::string hash = computeHash(!colorToMove); /* After the move, the opponent is to move */
    positionHistory[hash]++;
}

GameResult
Board::getGameResult(bool colorToMove)
{
    /* Fifty-move rule: 100 half-moves (50 full moves) without capture or pawn move */
    if (halfMoveClock >= 100)
    {
        return GameResult::DrawFiftyMove;
    }

    /* Threefold repetition: same position occurred 3 times */
    std::string currentHash = computeHash(colorToMove);
    if (positionHistory.count(currentHash) && positionHistory.at(currentHash) >= 3)
    {
        return GameResult::DrawThreefold;
    }

    std::vector<Move> legalMoves = getLegalMoves(colorToMove);

    if (!legalMoves.empty())
    {
        return GameResult::InProgress;
    }

    /* No legal moves: checkmate or stalemate */
    if (isInCheck(colorToMove))
    {
        return colorToMove ? GameResult::BlackWins : GameResult::WhiteWins;
    }

    return GameResult::DrawStalemate;
}

void
Board::print(std::ostream& os) const
{
    /* Print dead white pieces */
    for (auto p : whitePieces)
    {
        if (!p->isAlive())
        {
            os << *p;
        }
    }
    os << "\n";

    for (int Y = 1; Y <= BOARD_SIZE_MAX; Y++)
    {
        os << (BOARD_SIZE_MAX - Y + 1) << "| ";
        for (int X = 0; X < BOARD_SIZE_MAX; X++)
        {
            os << *board[X][BOARD_SIZE_MAX - Y];
        }
        os << "\n";
    }

    os << "___________________________\n";
    os << "  | a  b  c  d  e  f  g  h \n";

    /* Print dead black pieces */
    for (auto p : blackPieces)
    {
        if (!p->isAlive())
        {
            os << *p;
        }
    }
    os << "\n";
}

std::ostream&
operator<<(std::ostream& os, Board const& board)
{
    board.print(os);
    return os;
}
