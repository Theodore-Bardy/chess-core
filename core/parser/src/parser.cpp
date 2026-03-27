/**
 * @file  parser.cpp
 * @brief Parser sources — full PGN read/write with header tags and variations
 */

#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;

/* ── Token types ──────────────────────────────────────────────────────────── */

enum TokenType
{
    TOK_MOVE_NUM, /**< "1." or "1..." */
    TOK_MOVE,     /**< a move string, e.g. "e4", "Nxf3", "O-O", "e8=Q+" */
    TOK_LPAREN,   /**< "(" — start of variation */
    TOK_RPAREN,   /**< ")" — end of variation */
    TOK_COMMENT,  /**< text between braces { } */
    TOK_RESULT,   /**< "1-0", "0-1", "1/2-1/2", "*" */
    TOK_EOF
};

struct Token
{
    TokenType   type;
    std::string value;
};

/* ── Forward declarations ─────────────────────────────────────────────────── */

static std::vector<Token>   tokenize(std::string const& text);
static bool                 parseMoveToken(std::string const& tok, bool color,
                                           SquarePieceValue& pieceType, int& xEnd, int& yEnd,
                                           int& flags, char& extraFlag,
                                           SquarePieceValue& promotionPiece);
static void                 applyTokens(Game& game, std::vector<Token> const& tokens,
                                        size_t& pos, bool isVariation);
static int                  xFromChar(char c);
static int                  yFromChar(char c);
static std::string          moveToSAN(Move const& move);
static void                 writeNode(std::ostream& os, GameNode const* node,
                                      int& moveNum, bool& isWhiteToMove, bool firstInVariation);

/* ── Parser::loadGame ─────────────────────────────────────────────────────── */

bool
Parser::loadGame(Game& game, std::string const& filePath, GameMetadata* meta)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;
    std::string moveText;
    bool        moveSectionStarted = false;

    while (std::getline(file, line))
    {
        /* Trim trailing carriage-return (Windows line endings) */
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (line.empty())
        {
            continue;
        }

        /* Header line: [Tag "Value"] */
        if (line.front() == '[')
        {
            if (meta)
            {
                /* Parse tag name and value */
                size_t nameStart = 1;
                size_t nameEnd   = line.find(' ', nameStart);
                if (nameEnd == std::string::npos) continue;

                size_t valStart = line.find('"', nameEnd);
                size_t valEnd   = line.rfind('"');
                if (valStart == std::string::npos || valEnd <= valStart) continue;

                std::string tagName  = line.substr(nameStart, nameEnd - nameStart);
                std::string tagValue = line.substr(valStart + 1, valEnd - valStart - 1);

                if      (tagName == "Event")    meta->event    = tagValue;
                else if (tagName == "Site")     meta->site     = tagValue;
                else if (tagName == "Date")     meta->date     = tagValue;
                else if (tagName == "Round")    meta->round    = tagValue;
                else if (tagName == "White")    meta->white    = tagValue;
                else if (tagName == "Black")    meta->black    = tagValue;
                else if (tagName == "Result")   meta->result   = tagValue;
                else if (tagName == "WhiteElo") meta->whiteElo = std::stoi(tagValue.empty() ? "0" : tagValue);
                else if (tagName == "BlackElo") meta->blackElo = std::stoi(tagValue.empty() ? "0" : tagValue);
                else if (tagName == "ECO")      meta->eco      = tagValue;
                else if (tagName == "Opening")  meta->opening  = tagValue;
            }
            continue;
        }

        /* Move text: accumulate all non-header lines */
        moveSectionStarted = true;
        moveText += line + " ";
    }

    if (!moveSectionStarted || moveText.empty())
    {
        return true; /* empty game is OK */
    }

    /* Tokenise and apply */
    std::vector<Token> tokens = tokenize(moveText);
    size_t pos = 0;
    applyTokens(game, tokens, pos, false);

    return true;
}

/* ── Parser::saveGame ─────────────────────────────────────────────────────── */

bool
Parser::saveGame(Game const& game, std::string const& filePath, GameMetadata const* meta)
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    /* Write headers */
    auto writeTag = [&](std::string const& name, std::string const& value) {
        file << "[" << name << " \"" << value << "\"]\n";
    };

    if (meta)
    {
        if (!meta->event.empty())   writeTag("Event",    meta->event);
        if (!meta->site.empty())    writeTag("Site",     meta->site);
        if (!meta->date.empty())    writeTag("Date",     meta->date);
        if (!meta->round.empty())   writeTag("Round",    meta->round);
        if (!meta->white.empty())   writeTag("White",    meta->white);
        if (!meta->black.empty())   writeTag("Black",    meta->black);
        if (!meta->result.empty())  writeTag("Result",   meta->result);
        if (meta->whiteElo > 0)     writeTag("WhiteElo", std::to_string(meta->whiteElo));
        if (meta->blackElo > 0)     writeTag("BlackElo", std::to_string(meta->blackElo));
        if (!meta->eco.empty())     writeTag("ECO",      meta->eco);
        if (!meta->opening.empty()) writeTag("Opening",  meta->opening);
    }
    else
    {
        writeTag("Event", "?");
        writeTag("Site",  "?");
        writeTag("Date",  "????.??.??");
        writeTag("Round", "?");
        writeTag("White", "?");
        writeTag("Black", "?");
        writeTag("Result", "*");
    }

    file << "\n";

    /* Write move text by walking the game tree */
    GameNode const* root = game.getTree().getRoot();
    if (!root->children.empty())
    {
        int  moveNum      = 1;
        bool isWhiteToMove = true;
        writeNode(file, root->children[0], moveNum, isWhiteToMove, false);
    }

    /* Result token */
    if (meta && !meta->result.empty())
        file << " " << meta->result;
    else
        file << " *";

    file << "\n";
    return true;
}

/* ── Tokenizer ────────────────────────────────────────────────────────────── */

static std::vector<Token>
tokenize(std::string const& text)
{
    std::vector<Token> tokens;
    size_t i   = 0;
    size_t len = text.size();

    while (i < len)
    {
        /* Skip whitespace */
        while (i < len && std::isspace((unsigned char)text[i]))
        {
            i++;
        }
        if (i >= len) break;

        char c = text[i];

        /* Variation markers */
        if (c == '(') { tokens.push_back({TOK_LPAREN, "("}); i++; continue; }
        if (c == ')') { tokens.push_back({TOK_RPAREN, ")"}); i++; continue; }

        /* Comment: collect until '}' */
        if (c == '{')
        {
            i++;
            size_t start = i;
            while (i < len && text[i] != '}') i++;
            std::string comment = text.substr(start, i - start);
            /* Trim leading/trailing spaces from comment */
            size_t cs = comment.find_first_not_of(" \t\r\n");
            size_t ce = comment.find_last_not_of(" \t\r\n");
            if (cs != std::string::npos)
                comment = comment.substr(cs, ce - cs + 1);
            else
                comment.clear();
            tokens.push_back({TOK_COMMENT, comment});
            if (i < len) i++; /* skip '}' */
            continue;
        }

        /* NAG ($N): skip the entire NAG token */
        if (c == '$')
        {
            i++;
            while (i < len && std::isdigit((unsigned char)text[i])) i++;
            continue;
        }

        /* Collect a non-space token */
        size_t start = i;
        while (i < len && !std::isspace((unsigned char)text[i]) && text[i] != '(' && text[i] != ')')
        {
            i++;
        }
        std::string tok = text.substr(start, i - start);
        if (tok.empty()) continue;

        /* Result tokens */
        if (tok == "1-0" || tok == "0-1" || tok == "1/2-1/2" || tok == "*")
        {
            tokens.push_back({TOK_RESULT, tok});
            continue;
        }

        /* Move number: digit(s) followed by '.' (possibly more dots) */
        if (std::isdigit((unsigned char)tok[0]))
        {
            /* Could be a move number "1." "1..." or result "1/2-1/2" (handled above) */
            bool allDigitsDots = true;
            for (char ch : tok)
            {
                if (!std::isdigit((unsigned char)ch) && ch != '.') { allDigitsDots = false; break; }
            }
            if (allDigitsDots)
            {
                tokens.push_back({TOK_MOVE_NUM, tok});
                continue;
            }
        }

        /* Everything else is a move */
        tokens.push_back({TOK_MOVE, tok});
    }

    tokens.push_back({TOK_EOF, ""});
    return tokens;
}

/* ── Token application (recursive for variations) ────────────────────────── */

static void
applyTokens(Game& game, std::vector<Token> const& tokens, size_t& pos, bool isVariation)
{
    while (pos < tokens.size())
    {
        Token const& tok = tokens[pos];

        if (tok.type == TOK_EOF)
        {
            break;
        }

        if (tok.type == TOK_RESULT)
        {
            pos++;
            break;
        }

        if (tok.type == TOK_MOVE_NUM)
        {
            pos++;
            continue;
        }

        if (tok.type == TOK_COMMENT)
        {
            game.setComment(tok.value);
            pos++;
            continue;
        }

        if (tok.type == TOK_RPAREN)
        {
            if (isVariation)
            {
                pos++;
            }
            /* If not in a variation, ignore stray ')' */
            return;
        }

        if (tok.type == TOK_LPAREN)
        {
            pos++;
            game.startVariation();
            applyTokens(game, tokens, pos, true);
            game.endVariation();
            continue;
        }

        if (tok.type == TOK_MOVE)
        {
            pos++;

            SquarePieceValue pieceType     = SquarePieceValue::PawnValue;
            int              xEnd          = 0;
            int              yEnd          = 0;
            int              flags         = 0;
            char             extraFlag     = ' ';
            SquarePieceValue promotionPiece = SquarePieceValue::Empty;
            bool             color          = game.getColorToMove();

            if (parseMoveToken(tok.value, color, pieceType, xEnd, yEnd, flags, extraFlag, promotionPiece))
            {
                game.checkMove(pieceType,
                               color ? SquarePieceColor::White : SquarePieceColor::Black,
                               xEnd, yEnd, flags, extraFlag, promotionPiece);
            }
            continue;
        }

        /* Unknown token type — skip */
        pos++;
    }
}

/* ── Move token parser ────────────────────────────────────────────────────── */

static int
xFromChar(char c)
{
    switch (c)
    {
        case 'a': return SQUARE_X_A;
        case 'b': return SQUARE_X_B;
        case 'c': return SQUARE_X_C;
        case 'd': return SQUARE_X_D;
        case 'e': return SQUARE_X_E;
        case 'f': return SQUARE_X_F;
        case 'g': return SQUARE_X_G;
        case 'h': return SQUARE_X_H;
        default:  return -1;
    }
}

static int
yFromChar(char c)
{
    switch (c)
    {
        case '1': return SQUARE_Y_1;
        case '2': return SQUARE_Y_2;
        case '3': return SQUARE_Y_3;
        case '4': return SQUARE_Y_4;
        case '5': return SQUARE_Y_5;
        case '6': return SQUARE_Y_6;
        case '7': return SQUARE_Y_7;
        case '8': return SQUARE_Y_8;
        default:  return -1;
    }
}

static SquarePieceValue
pieceCharToValue(char c)
{
    switch (c)
    {
        case 'Q': return SquarePieceValue::QueenValue;
        case 'R': return SquarePieceValue::RookValue;
        case 'B': return SquarePieceValue::BishopValue;
        case 'N': return SquarePieceValue::KnightValue;
        default:  return SquarePieceValue::Empty;
    }
}

static bool
parseMoveToken(std::string const& tok, bool color,
               SquarePieceValue& pieceType, int& xEnd, int& yEnd,
               int& flags, char& extraFlag, SquarePieceValue& promotionPiece)
{
    flags          = 0;
    extraFlag      = ' ';
    promotionPiece = SquarePieceValue::Empty;

    /* Strip trailing check/checkmate/annotation characters (+, #, !, ?) */
    std::string s = tok;
    while (!s.empty() && (s.back() == '+' || s.back() == '#' || s.back() == '!' || s.back() == '?'))
    {
        s.pop_back();
    }
    if (s.empty()) return false;

    /* Queen-side castle */
    if (s == "O-O-O")
    {
        flags    |= MOVE_FLAG_QUEEN_CASTLE;
        xEnd      = KING_QUEEN_CASTLE_X;
        yEnd      = color ? KING_WHITE_DEFAULT_Y : KING_BLACK_DEFAULT_Y;
        pieceType = SquarePieceValue::KingValue;
        return true;
    }

    /* King-side castle */
    if (s == "O-O")
    {
        flags    |= MOVE_FLAG_KING_CASTLE;
        xEnd      = KING_KING_CASTLE_X;
        yEnd      = color ? KING_WHITE_DEFAULT_Y : KING_BLACK_DEFAULT_Y;
        pieceType = SquarePieceValue::KingValue;
        return true;
    }

    /* Check for promotion: "=X" suffix */
    size_t eqPos = s.find('=');
    if (eqPos != std::string::npos)
    {
        if (eqPos + 1 < s.size())
        {
            promotionPiece = pieceCharToValue(s[eqPos + 1]);
        }
        flags |= MOVE_FLAG_PROMOTION;
        s = s.substr(0, eqPos); /* strip "=Q" part */
    }

    /* Pawn move: first character is lowercase (a-h) */
    if (std::islower((unsigned char)s[0]))
    {
        pieceType = SquarePieceValue::PawnValue;

        if (s.size() >= 4 && s[1] == 'x')
        {
            /* Pawn capture: "exd5" */
            flags    |= MOVE_FLAG_TAKE;
            extraFlag = s[0]; /* source file for disambiguation */
            xEnd      = xFromChar(s[2]);
            yEnd      = yFromChar(s[3]);
        }
        else if (s.size() >= 2)
        {
            /* Simple pawn move: "e4" */
            xEnd = xFromChar(s[0]);
            yEnd = yFromChar(s[1]);
        }
        else
        {
            return false;
        }
        return (xEnd >= 0 && yEnd >= 0);
    }

    /* Piece move: first character is uppercase (K, Q, B, N, R) */
    switch (s[0])
    {
        case 'K': pieceType = SquarePieceValue::KingValue;   break;
        case 'Q': pieceType = SquarePieceValue::QueenValue;  break;
        case 'B': pieceType = SquarePieceValue::BishopValue; break;
        case 'N': pieceType = SquarePieceValue::KnightValue; break;
        case 'R': pieceType = SquarePieceValue::RookValue;   break;
        default:  return false;
    }

    /* Rest of the string after the piece letter */
    std::string rest = s.substr(1);

    /* Find 'x' for capture */
    size_t xCapPos = rest.find('x');

    if (xCapPos == std::string::npos)
    {
        /* No capture */
        if (rest.size() == 2)
        {
            /* "Nd4" — destination only */
            xEnd = xFromChar(rest[0]);
            yEnd = yFromChar(rest[1]);
        }
        else if (rest.size() == 3)
        {
            /* "Nbd4" or "N1d4" — one disambiguator + destination */
            extraFlag = rest[0];
            xEnd      = xFromChar(rest[1]);
            yEnd      = yFromChar(rest[2]);
        }
        else if (rest.size() == 4)
        {
            /* "Nb1d4" — full-square disambiguator (very rare); use file only */
            extraFlag = rest[0];
            xEnd      = xFromChar(rest[2]);
            yEnd      = yFromChar(rest[3]);
        }
        else
        {
            return false;
        }
    }
    else
    {
        /* Capture */
        flags |= MOVE_FLAG_TAKE;

        if (xCapPos == 0)
        {
            /* "Nxd4" — capture, no disambiguator */
            if (rest.size() < 3) return false;
            xEnd = xFromChar(rest[1]);
            yEnd = yFromChar(rest[2]);
        }
        else if (xCapPos == 1)
        {
            /* "Nbxd4" or "N1xd4" — one disambiguator + capture */
            extraFlag = rest[0];
            if (rest.size() < 4) return false;
            xEnd = xFromChar(rest[2]);
            yEnd = yFromChar(rest[3]);
        }
        else if (xCapPos == 2)
        {
            /* "Nb1xd4" — full-square disambiguator + capture; use file only */
            extraFlag = rest[0];
            if (rest.size() < 5) return false;
            xEnd = xFromChar(rest[3]);
            yEnd = yFromChar(rest[4]);
        }
        else
        {
            return false;
        }
    }

    return (xEnd >= 0 && yEnd >= 0);
}

/* ── SAN generator ───────────────────────────────────────────────────────── */

static char xToChar(int x)
{
    return (char)('a' + x);
}

static char yToChar(int y)
{
    return (char)('1' + y);
}

static char pieceValueToChar(SquarePieceValue val)
{
    switch (val)
    {
        case SquarePieceValue::KingValue:   return 'K';
        case SquarePieceValue::QueenValue:  return 'Q';
        case SquarePieceValue::RookValue:   return 'R';
        case SquarePieceValue::BishopValue: return 'B';
        case SquarePieceValue::KnightValue: return 'N';
        default: return '\0';
    }
}

static std::string
moveToSAN(Move const& move)
{
    int  flags = move.getFlags();
    int  xEnd  = move.getXEnd();
    int  yEnd  = move.getYEnd();

    /* Castling */
    if (flags & MOVE_FLAG_KING_CASTLE)  return "O-O";
    if (flags & MOVE_FLAG_QUEEN_CASTLE) return "O-O-O";

    Piece* piece = move.getPiece();
    if (!piece) return "?";

    SquarePieceValue pieceVal = piece->getValue();
    std::string san;

    if (pieceVal == SquarePieceValue::PawnValue)
    {
        /* Pawn capture: include source file */
        if (flags & MOVE_FLAG_TAKE)
        {
            san += xToChar(move.getXStart());
            san += 'x';
        }
        san += xToChar(xEnd);
        san += yToChar(yEnd);

        /* Promotion */
        if (flags & MOVE_FLAG_PROMOTION)
        {
            san += '=';
            char pc = pieceValueToChar(move.getPromotionPiece());
            if (pc) san += pc;
        }
    }
    else
    {
        /* Piece letter */
        san += pieceValueToChar(pieceVal);

        /* Disambiguation (stored as extraFlag during parsing) */
        char extra = move.getExtraFlag();
        if (extra != ' ') san += extra;

        /* Capture */
        if (flags & MOVE_FLAG_TAKE) san += 'x';

        /* Destination */
        san += xToChar(xEnd);
        san += yToChar(yEnd);
    }

    return san;
}

/* ── PGN writer ──────────────────────────────────────────────────────────── */

static void
writeNode(std::ostream& os, GameNode const* node, int& moveNum, bool& isWhiteToMove, bool firstInVariation)
{
    if (!node) return;

    /* Move number prefix */
    if (isWhiteToMove)
    {
        os << moveNum << ". ";
    }
    else if (firstInVariation)
    {
        /* Black move that starts a variation: needs "N..." for context */
        os << moveNum << "... ";
    }

    /* Print the move in standard algebraic notation */
    os << moveToSAN(node->move);

    /* Comment */
    if (!node->comment.empty())
    {
        os << " {" << node->comment << "}";
    }

    /* Advance clocks */
    bool wasWhite = isWhiteToMove;
    isWhiteToMove = !isWhiteToMove;
    if (!wasWhite)
    {
        moveNum++;
    }

    /* Write alternative variations (children[1..]) before continuing main line */
    for (size_t i = 1; i < node->children.size(); i++)
    {
        int  varMoveNum      = wasWhite ? moveNum - 1 : moveNum - 1;
        bool varIsWhiteToMove = wasWhite;
        /* After white's move, variation shows alternative for white (same move number) */
        /* After black's move, variation shows alternative for black (same move number - 1) */
        if (wasWhite)
        {
            varMoveNum = moveNum - 1; /* undo the increment — wasWhite means we just printed white */
        }
        else
        {
            varMoveNum = moveNum - 1;
        }

        os << " (";
        writeNode(os, node->children[i], varMoveNum, varIsWhiteToMove, true);
        os << ")";
    }

    /* Continue main line */
    if (!node->children.empty())
    {
        os << " ";
        writeNode(os, node->children[0], moveNum, isWhiteToMove, false);
    }
}
