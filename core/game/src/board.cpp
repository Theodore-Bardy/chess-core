/**
 * @file  board.cpp
 * @brief Board sources
 */

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
{

    /* Copy squares */
    /* Initialize empty board */
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
        board[KING_WHITE_DEFAULT_X][KING_WHITE_DEFAULT_Y]->setValue(SquareValue::KingValue);
        board[KING_WHITE_DEFAULT_X][KING_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_king);
        whiteKing = white_king;

        Queen* white_queen = new Queen(PIECE_WHITE);
        board[QUEEN_WHITE_DEFAULT_X][QUEEN_WHITE_DEFAULT_Y]->setValue(SquareValue::QueenValue);
        board[QUEEN_WHITE_DEFAULT_X][QUEEN_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_queen);
        whiteQueen = white_queen;

        Bishop* white_bishop = new Bishop(PIECE_WHITE, BISHOP_1_WHITE_DEFAULT_X, BISHOP_1_WHITE_DEFAULT_Y);
        board[BISHOP_1_WHITE_DEFAULT_X][BISHOP_1_WHITE_DEFAULT_Y]->setValue(SquareValue::BishopValue);
        board[BISHOP_1_WHITE_DEFAULT_X][BISHOP_1_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_bishop);
        whiteBishops[0] = white_bishop;

        white_bishop = new Bishop(PIECE_WHITE, BISHOP_2_WHITE_DEFAULT_X, BISHOP_2_WHITE_DEFAULT_Y);
        board[BISHOP_2_WHITE_DEFAULT_X][BISHOP_2_WHITE_DEFAULT_Y]->setValue(SquareValue::BishopValue);
        board[BISHOP_2_WHITE_DEFAULT_X][BISHOP_2_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_bishop);
        whiteBishops[1] = white_bishop;

        Knight* white_knight = new Knight(PIECE_WHITE, KNIGHT_1_WHITE_DEFAULT_X, KNIGHT_1_WHITE_DEFAULT_Y);
        board[KNIGHT_1_WHITE_DEFAULT_X][KNIGHT_1_WHITE_DEFAULT_Y]->setValue(SquareValue::KnightValue);
        board[KNIGHT_1_WHITE_DEFAULT_X][KNIGHT_1_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_knight);
        whiteKnights[0] = white_knight;

        white_knight = new Knight(PIECE_WHITE, KNIGHT_2_WHITE_DEFAULT_X, KNIGHT_2_WHITE_DEFAULT_Y);
        board[KNIGHT_2_WHITE_DEFAULT_X][KNIGHT_2_WHITE_DEFAULT_Y]->setValue(SquareValue::KnightValue);
        board[KNIGHT_2_WHITE_DEFAULT_X][KNIGHT_2_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_knight);
        whiteKnights[1] = white_knight;

        Rook* white_rook = new Rook(PIECE_WHITE, ROOK_1_WHITE_DEFAULT_X, ROOK_1_WHITE_DEFAULT_Y);
        board[ROOK_1_WHITE_DEFAULT_X][ROOK_1_WHITE_DEFAULT_Y]->setValue(SquareValue::RookValue);
        board[ROOK_1_WHITE_DEFAULT_X][ROOK_1_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_rook);
        whiteRooks[0] = white_rook;

        white_rook = new Rook(PIECE_WHITE, ROOK_2_WHITE_DEFAULT_X, ROOK_2_WHITE_DEFAULT_Y);
        board[ROOK_2_WHITE_DEFAULT_X][ROOK_2_WHITE_DEFAULT_Y]->setValue(SquareValue::RookValue);
        board[ROOK_2_WHITE_DEFAULT_X][ROOK_2_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
        whitePieces.push_back(white_rook);
        whiteRooks[1] = white_rook;

        /* Black pieces */
        King* black_king = new King(PIECE_BLACK);
        board[KING_BLACK_DEFAULT_X][KING_BLACK_DEFAULT_Y]->setValue(SquareValue::KingValue);
        board[KING_BLACK_DEFAULT_X][KING_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_king);
        blackKing = black_king;

        Queen* black_queen = new Queen(PIECE_BLACK);
        board[QUEEN_BLACK_DEFAULT_X][QUEEN_BLACK_DEFAULT_Y]->setValue(SquareValue::QueenValue);
        board[QUEEN_BLACK_DEFAULT_X][QUEEN_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_queen);
        blackQueen = black_queen;

        Bishop* black_bishop = new Bishop(PIECE_BLACK, BISHOP_1_BLACK_DEFAULT_X, BISHOP_1_BLACK_DEFAULT_Y);
        board[BISHOP_1_BLACK_DEFAULT_X][BISHOP_1_BLACK_DEFAULT_Y]->setValue(SquareValue::BishopValue);
        board[BISHOP_1_BLACK_DEFAULT_X][BISHOP_1_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_bishop);
        blackBishops[0] = black_bishop;

        black_bishop = new Bishop(PIECE_BLACK, BISHOP_2_BLACK_DEFAULT_X, BISHOP_2_BLACK_DEFAULT_Y);
        board[BISHOP_2_BLACK_DEFAULT_X][BISHOP_2_BLACK_DEFAULT_Y]->setValue(SquareValue::BishopValue);
        board[BISHOP_2_BLACK_DEFAULT_X][BISHOP_2_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_bishop);
        blackBishops[1] = black_bishop;

        Knight* black_knight = new Knight(PIECE_BLACK, KNIGHT_1_BLACK_DEFAULT_X, KNIGHT_1_BLACK_DEFAULT_Y);
        board[KNIGHT_1_BLACK_DEFAULT_X][KNIGHT_1_BLACK_DEFAULT_Y]->setValue(SquareValue::KnightValue);
        board[KNIGHT_1_BLACK_DEFAULT_X][KNIGHT_1_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_knight);
        blackKnights[0] = black_knight;

        black_knight = new Knight(PIECE_BLACK, KNIGHT_2_BLACK_DEFAULT_X, KNIGHT_2_BLACK_DEFAULT_Y);
        board[KNIGHT_2_BLACK_DEFAULT_X][KNIGHT_2_BLACK_DEFAULT_Y]->setValue(SquareValue::KnightValue);
        board[KNIGHT_2_BLACK_DEFAULT_X][KNIGHT_2_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_knight);
        blackKnights[1] = black_knight;

        Rook* black_rook = new Rook(PIECE_BLACK, ROOK_1_BLACK_DEFAULT_X, ROOK_1_BLACK_DEFAULT_Y);
        board[ROOK_1_BLACK_DEFAULT_X][ROOK_1_BLACK_DEFAULT_Y]->setValue(SquareValue::RookValue);
        board[ROOK_1_BLACK_DEFAULT_X][ROOK_1_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_rook);
        blackRooks[0] = black_rook;

        black_rook = new Rook(PIECE_BLACK, ROOK_2_BLACK_DEFAULT_X, ROOK_2_BLACK_DEFAULT_Y);
        board[ROOK_2_BLACK_DEFAULT_X][ROOK_2_BLACK_DEFAULT_Y]->setValue(SquareValue::RookValue);
        board[ROOK_2_BLACK_DEFAULT_X][ROOK_2_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
        blackPieces.push_back(black_rook);
        blackRooks[1] = black_rook;

        /* Pawns */
        for (int index = 0; index < BOARD_SIZE_MAX; index++)
        {
            /* White pawns */
            Pawn* white_pawn = new Pawn(PIECE_WHITE, index);
            board[index][PAWN_WHITE_DEFAULT_Y]->setValue(SquareValue::PawnValue);
            board[index][PAWN_WHITE_DEFAULT_Y]->setColor(SquarePieceColor::White);
            whitePieces.push_back(white_pawn);
            whitePawns[index] = white_pawn;

            /* Black pawns */
            Pawn* black_pawn = new Pawn(PIECE_BLACK, index);
            board[index][PAWN_BLACK_DEFAULT_Y]->setValue(SquareValue::PawnValue);
            board[index][PAWN_BLACK_DEFAULT_Y]->setColor(SquarePieceColor::Black);
            blackPieces.push_back(black_pawn);
            blackPawns[index] = black_pawn;
        }
    }
}

bool
Board::movePiece(Piece* piece, int x, int y, int flags)
{
    int oldX = piece->getX();
    int oldY = piece->getY();

    /* Compute flags - take */
    if ((MOVE_FLAG_EAT == (flags & MOVE_FLAG_EAT)))
    {
        Piece* p = nullptr;
        if (this->selectPiece(&p, x, y))
        {
            piece->eat(*p); // TODO - eat is not to best method

            /* Reset square */
            board[x][y]->setValue(SquareValue::Empty);
            board[x][y]->setColor(SquarePieceColor::NoPiece);
        }
    }

    if (piece->move(x, y, flags))
    {
        /* Reset old square */
        board[oldX][oldY]->setValue(SquareValue::Empty);
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

        return true;
    }

    return false;
}

bool
Board::selectPiece(Piece** piece, int x, int y)
{
    if ((board[x][y]->getColor() != SquarePieceColor::NoPiece) && (board[x][y]->getValue() != SquareValue::Empty))
    {
        if (board[x][y]->getColor() == SquarePieceColor::White)
        {
            switch (board[x][y]->getValue())
            {
                case SquareValue::KingValue:
                    *piece = whiteKing;
                    break;

                case SquareValue::QueenValue:
                    *piece = whiteQueen;
                    break;

                case SquareValue::BishopValue:
                    if ((whiteBishops[0]->getX() == x) && (whiteBishops[0]->getY() == y))
                    {
                        *piece = whiteBishops[0];
                    }
                    else
                    {
                        *piece = whiteBishops[1];
                    }
                    break;

                case SquareValue::KnightValue:
                    if ((whiteKnights[0]->getX() == x) && (whiteKnights[0]->getY() == y))
                    {
                        *piece = whiteKnights[0];
                    }
                    else
                    {
                        *piece = whiteKnights[1];
                    }
                    break;

                case SquareValue::RookValue:
                    if ((whiteRooks[0]->getX() == x) && (whiteRooks[0]->getY() == y))
                    {
                        *piece = whiteRooks[0];
                    }
                    else
                    {
                        *piece = whiteRooks[1];
                    }
                    break;

                case SquareValue::PawnValue:
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
                case SquareValue::KingValue:
                    *piece = blackKing;
                    break;

                case SquareValue::QueenValue:
                    *piece = blackQueen;
                    break;

                case SquareValue::BishopValue:
                    if ((blackBishops[0]->getX() == x) && (blackBishops[0]->getY() == y))
                    {
                        *piece = blackBishops[0];
                    }
                    else
                    {
                        *piece = blackBishops[1];
                    }
                    break;

                case SquareValue::KnightValue:
                    if ((blackKnights[0]->getX() == x) && (blackKnights[0]->getY() == y))
                    {
                        *piece = blackKnights[0];
                    }
                    else
                    {
                        *piece = blackKnights[1];
                    }
                    break;

                case SquareValue::RookValue:
                    if ((blackRooks[0]->getX() == x) && (blackRooks[0]->getY() == y))
                    {
                        *piece = blackRooks[0];
                    }
                    else
                    {
                        *piece = blackRooks[1];
                    }
                    break;

                case SquareValue::PawnValue:
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
Board::lookForPiecesAbleToMoveAt(std::vector<Piece*>& pieces, SquareValue pieceType, SquarePieceColor pieceColor, int x, int y, int flags)
{
    bool pieceFound = false;

    /* White */
    if (pieceColor == White)
    {
        for (auto p : whitePieces)
        {
            if ((p->getValue() == pieceType) && p->isAlive() && p->isAbleToMove(x, y, flags))
            {
                pieces.push_back(p);
                pieceFound = true;
            }
        }
    }
    /* Black */
    else
    {
        for (auto p : blackPieces)
        {
            if ((p->getValue() == pieceType) && p->isAlive() && p->isAbleToMove(x, y, flags))
            {
                pieces.push_back(p);
                pieceFound = true;
            }
        }
    }

    return pieceFound;
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
