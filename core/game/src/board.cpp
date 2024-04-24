/**
 * @file  board.cpp
 * @brief Board sources
 */

#include <assert.h>
#include "board.hpp"
#include "move.hpp"
#include "utils.hpp"

Board::Board()
    : isInitialize(false)
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
Board::startUp(void)
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
Board::movePiece(Piece* piece, int x, int y, int& flags)
{
    /* Check parameter */
    assert(nullptr != piece);

    bool xReturn = false;
    int  oldX    = piece->getX();
    int  oldY    = piece->getY();

    /* Clear flag parameter */
    flags = 0;

    /* Check move and compute flags */
    xReturn = piece->checkMove(x, y, flags, board);

    /* Compute flags - king castle */
    if (xReturn && (MOVE_FLAG_KING_CASTLE == (flags & MOVE_FLAG_KING_CASTLE)))
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

        /* Check square on the king way aren't attacked */
        if (!this->isSquareAttacked(!piece->getColor(), KING_KING_CASTLE_X, y) && !this->isSquareAttacked(!piece->getColor(), ROOK_KING_CASTLE_X, y))
        {
            xReturn = k->castle(true, r);
            if (xReturn)
            {
                /* White */
                if (piece->getColor())
                {
                    board[KING_WHITE_DEFAULT_X][KING_WHITE_DEFAULT_Y]->resetSquare();
                    board[ROOK_2_WHITE_DEFAULT_X][ROOK_2_WHITE_DEFAULT_Y]->resetSquare();
                    board[k->getX()][k->getY()]->setColor(SquarePieceColor::White);
                    board[r->getX()][r->getY()]->setColor(SquarePieceColor::White);
                }
                /* Black */
                else
                {
                    board[KING_BLACK_DEFAULT_X][KING_BLACK_DEFAULT_Y]->resetSquare();
                    board[ROOK_2_BLACK_DEFAULT_X][ROOK_2_BLACK_DEFAULT_Y]->resetSquare();
                    board[k->getX()][k->getY()]->setColor(SquarePieceColor::Black);
                    board[r->getX()][r->getY()]->setColor(SquarePieceColor::Black);
                }
                board[k->getX()][k->getY()]->setValue(SquarePieceValue::KingValue);
                board[r->getX()][r->getY()]->setValue(SquarePieceValue::RookValue);
            }
        }
        else
        {
            xReturn = false;
        }
    }

    /* Compute flags - queen castle */
    else if (xReturn && (MOVE_FLAG_QUEEN_CASTLE == (flags & MOVE_FLAG_QUEEN_CASTLE)))
    {
        Rook* r = nullptr;
        King* k = (piece->getColor()) ? whiteKing : blackKing;
        assert(k == piece);

        /* Find queen side rook */
        if (piece->getColor())
        {
            r = (!whiteRooks[0]->isKingSideRook()) ? whiteRooks[0] : whiteRooks[1];
        }
        else
        {
            r = (!blackRooks[0]->isKingSideRook()) ? blackRooks[0] : blackRooks[1];
        }

        /* Check square on the king way aren't attacked */
        if (!this->isSquareAttacked(!piece->getColor(), KING_QUEEN_CASTLE_X, y) && !this->isSquareAttacked(!piece->getColor(), ROOK_QUEEN_CASTLE_X, y))
        {
            xReturn = k->castle(false, r);
            if (xReturn)
            {
                /* White */
                if (piece->getColor())
                {
                    board[KING_WHITE_DEFAULT_X][KING_WHITE_DEFAULT_Y]->resetSquare();
                    board[ROOK_1_WHITE_DEFAULT_X][ROOK_1_WHITE_DEFAULT_Y]->resetSquare();
                    board[k->getX()][k->getY()]->setColor(SquarePieceColor::White);
                    board[r->getX()][r->getY()]->setColor(SquarePieceColor::White);
                }
                /* Black */
                else
                {
                    board[KING_BLACK_DEFAULT_X][KING_BLACK_DEFAULT_Y]->resetSquare();
                    board[ROOK_1_BLACK_DEFAULT_X][ROOK_1_BLACK_DEFAULT_Y]->resetSquare();
                    board[k->getX()][k->getY()]->setColor(SquarePieceColor::Black);
                    board[r->getX()][r->getY()]->setColor(SquarePieceColor::Black);
                }
                board[k->getX()][k->getY()]->setValue(SquarePieceValue::KingValue);
                board[r->getX()][r->getY()]->setValue(SquarePieceValue::RookValue);
            }
        }
        else
        {
            xReturn = false;
        }
    }

    /* Normal move */
    else if (xReturn)
    {
        /* Check move is legal - will not put the king in check state 
         * 1. Save destination square configuration
         * 2. Simulate the move on the board by modifying start and dest squares
         * 3. Check if king is attacked
         * 4. Restore board as before
         */
        SquarePieceColor saveColor = board[x][y]->getColor();
        SquarePieceValue saveValue = board[x][y]->getValue();
        King*            k         = (piece->getColor()) ? whiteKing : blackKing;
        board[piece->getX()][piece->getY()]->resetSquare();
        board[x][y]->setColor((piece->getColor()) ? SquarePieceColor::White : SquarePieceColor::Black);
        board[x][y]->setValue(piece->getValue());
        if (isSquareAttacked(!piece->getColor(), k->getX(), k->getY()))
        {
            xReturn = false;
        }
        board[x][y]->setColor(saveColor);
        board[x][y]->setValue(saveValue);
        board[piece->getX()][piece->getY()]->setColor((piece->getColor()) ? SquarePieceColor::White : SquarePieceColor::Black);
        board[piece->getX()][piece->getY()]->setValue(piece->getValue());

        /* Compute flags - take */
        if (xReturn && (MOVE_FLAG_TAKE == (flags & MOVE_FLAG_TAKE)))
        {
            Piece* p = nullptr;
            if (this->selectPiece(&p, x, y))
            {
                /* Do not reset square otherwise the move() method fail */
                piece->take(*p);
            }
            else
            {
                xReturn = false;
            }
        }

        /* Move piece */
        if (xReturn && piece->move(x, y, flags, board))
        {
            /* Reset old square */
            board[oldX][oldY]->resetSquare();

            /* Fill new square */
            board[x][y]->setColor((piece->getColor()) ? SquarePieceColor::White : SquarePieceColor::Black);
            board[x][y]->setValue(piece->getValue());
        }
    }

    /* Compute flags - check: check if opposite color king is check */
    King* k = (piece->getColor()) ? blackKing : whiteKing;
    if (xReturn && this->isSquareAttacked(piece->getColor(), k->getX(), k->getY()))
    {
        k->setCheckStatus(true);
        flags |= MOVE_FLAG_CHECK;

        /* To check if a king is mate:
         * 1. Check if the king square is attacked
         *  1. true means the king is at least in check state, see 2.
         *  2. false means the king isn't mate (and check).
         * 2. Check for all squares around the king are attacked:
         *  1. true means the king can't move so check 3.
         *  2. false means the king can try to move on a safe square, check if the king can legally move to a those squares.
         *    1. true means the king isn't mate
         *    2. false means the king can't move so check 3.
         * 3. Check if any piece can meddle in the opponent.
         */
        bool isMate   = true;
        int  flagTemp = 0;

        /* Check king moves */
        for (int i = -1; i < 2; ++i)
        {
            for (int j = -1; j < 2; ++j)
            {
                if (!this->isSquareAttacked(piece->getColor(), k->getX() + i, k->getY() + j)
                    && !this->isSquareDefended(piece->getColor(), k->getX() + i, k->getY() + j) && k->checkMove(k->getX() + i, k->getY() + j, flagTemp, board))
                {
                    isMate = false;
                    break;
                }
            }

            if (!isMate)
            {
                break;
            }
        }

        /* Check others pieces moves */
        if (isMate)
        {
            /* For all alive opponent pieces, check all available moves and if the king is still check */
            for (auto p : (piece->getColor() ? blackPieces : whitePieces))
            {
                if (p->isAlive() && (p != k))
                {
                    /* Save current position of the piece because it will move to simulate a new board */
                    int savePx = p->getX();
                    int savePy = p->getY();

                    for (auto xy : p->getAllMoves(board))
                    {
                        /* Go to the xy found position */
                        // TODO - some moves can be undo (pawn or first moves)
                        p->move(xy.first, xy.second, flagTemp, board);

                        /* Check if king is still attacked */
                        if (!this->isSquareAttacked(piece->getColor(), k->getX(), k->getY()))
                        {
                            isMate = false;
                        }

                        /* Return to saved position */
                        p->move(savePx, savePy, flagTemp, board);
                    }

                    /* Check if one move that can avoid check mate was found */
                    if (!isMate)
                    {
                        break;
                    }
                }
            }
        }

        if (isMate)
        {
            k->setMateStatus(true);
            flags |= MOVE_FLAG_CHECK_MATE;
        }
    }

    return xReturn;
}

bool
Board::selectPiece(Piece** piece, int x, int y)
{
    if ((board[x][y]->getColor() != SquarePieceColor::NoPiece) && (board[x][y]->getValue() != SquarePieceValue::Empty))
    {
        /* Look for white piece */
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
                    for (auto p : whitePawns)
                    {
                        if ((p->getX() == x) && (p->getY() == y))
                        {
                            *piece = p;
                            break;
                        }
                    }
                    break;

                default:
                    /* Should not be here */
                    LOG_ERROR("Try to select a white piece with unknown value.");
                    assert(false);
                    return false;
            }
        }
        /* Look for black piece */
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
                    for (auto p : blackPawns)
                    {
                        if ((p->getX() == x) && (p->getY() == y))
                        {
                            *piece = p;
                            break;
                        }
                    }
                    break;

                default:
                    /* Should not be here */
                    LOG_ERROR("Try to select a black piece with unknown value.");
                    assert(false);
                    return false;
            }
        }
        return true;
    }

    return false;
}

bool
Board::searchPiece(Piece** piece, SquarePieceValue pieceType, SquarePieceColor pieceColor, int x, int y, int flags, char extraFlag)
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

    /* Lambda declaration - search a particular piece */
    auto searchPiece
        = [&piece, &pieceType, &x, &y, &flags, &xExtraFlag, &yExtraFlag](vector<Piece*> const& pieces, Square* board[BOARD_SIZE_MAX][BOARD_SIZE_MAX]) {
              for (auto p : pieces)
              {
                  if ((p->getValue() == pieceType) && p->isAlive() && p->checkMove(x, y, flags, board) && ((xExtraFlag == -1) || (p->getX() == xExtraFlag))
                      && ((yExtraFlag == -1) || (p->getY() == yExtraFlag)))
                  {
                      /* Only one piece should fit to the description */
                      assert(nullptr == *piece);
                      *piece = p;
                  }
              }
              return (*piece != nullptr);
          };

    return searchPiece(((pieceColor == White) ? whitePieces : blackPieces), board);
}

bool
Board::isSquareAttacked(bool pieceColor, int x, int y)
{
    int flags = 0;

    auto isAttacked = [&x, &y, &flags](vector<Piece*> const& pieces, Square* board[BOARD_SIZE_MAX][BOARD_SIZE_MAX]) {
        for (auto p : pieces)
        {
            if (p->isAlive() && p->checkMove(x, y, flags, board))
            {
                return true;
            }
        }
        return false;
    };

    return isAttacked(((pieceColor) ? whitePieces : blackPieces), board);
}

bool
Board::isSquareDefended(bool pieceColor, int x, int y)
{
    int flags = 0;

    auto isDefended = [&x, &y, &flags](vector<Piece*> const& pieces, Square* board[BOARD_SIZE_MAX][BOARD_SIZE_MAX]) {
        SquarePieceColor saveColor = board[x][y]->getColor();
        SquarePieceValue saveValue = board[x][y]->getValue();
        board[x][y]->resetSquare();
        for (auto p : pieces)
        {
            if (p->isAlive() && p->checkMove(x, y, flags, board))
            {
                board[x][y]->setColor(saveColor);
                board[x][y]->setValue(saveValue);
                return true;
            }
        }
        board[x][y]->setColor(saveColor);
        board[x][y]->setValue(saveValue);
        return false;
    };

    return isDefended(((pieceColor) ? whitePieces : blackPieces), board);
}

bool
Board::isKingCheck(bool kingColor)
{
    return ((kingColor) ? whiteKing->getCheckStatus() : blackKing->getCheckStatus());
}

bool
Board::isKingMate(bool kingColor)
{
    return ((kingColor) ? whiteKing->getMateStatus() : blackKing->getMateStatus());
}

bool
Board::isKingPat(bool kingColor)
{
    return ((kingColor) ? whiteKing->getPatStatus() : blackKing->getPatStatus());
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
