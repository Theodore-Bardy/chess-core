/**
 * @file  tui_app.cpp
 * @brief FTXUI terminal UI implementation
 */

#include "tui_app.hpp"

#include <algorithm>
#include <filesystem>
#include <sstream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;
namespace fs = std::filesystem;

/* ------------------------------------------------------------------ */
/* Construction                                                         */
/* ------------------------------------------------------------------ */

TuiApp::TuiApp()
{
    statusMsg = "Arrow keys = cursor | Enter = select/move | L = load | S = save | Q = quit";
}

/* ------------------------------------------------------------------ */
/* FEN helper                                                           */
/* ------------------------------------------------------------------ */

char TuiApp::pieceAt(std::string const& fen, int x, int y)
{
    /* FEN piece placement: rank 8 first (top), rank 1 last (bottom) */
    /* x=0=fileA..7=fileH, y=0=rank1..7=rank8 */
    int targetRankFromTop = 7 - y;
    int curRank = 0, curFile = 0;

    for (char c : fen)
    {
        if (c == ' ') break;
        if (c == '/') { ++curRank; curFile = 0; continue; }
        if (c >= '1' && c <= '8')
        {
            int skip = c - '0';
            if (curRank == targetRankFromTop && curFile <= x && x < curFile + skip)
                return '.';
            curFile += skip;
            continue;
        }
        if (curRank == targetRankFromTop && curFile == x) return c;
        ++curFile;
    }
    return '.';
}

/* ------------------------------------------------------------------ */
/* Board rendering (Canvas)                                             */
/* ------------------------------------------------------------------ */

static const int CELL_W = 6; /* canvas columns per square */
static const int CELL_H = 4; /* canvas rows per square    */

static std::string pieceGlyph(char p)
{
    switch (p)
    {
        case 'K': return "K"; case 'Q': return "Q";
        case 'R': return "R"; case 'B': return "B";
        case 'N': return "N"; case 'P': return "P";
        case 'k': return "k"; case 'q': return "q";
        case 'r': return "r"; case 'b': return "b";
        case 'n': return "n"; case 'p': return "p";
        default:  return " ";
    }
}

void TuiApp::renderBoardPanel(Canvas& c) const
{
    std::string fen = game.getFEN();

    for (int rank = 7; rank >= 0; --rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            int  cx = file * CELL_W;
            int  cy = (7 - rank) * CELL_H;
            bool lightSq   = (file + rank) % 2 != 0;
            bool isCursor  = (file == cursorX && rank == cursorY);
            bool isSelected= (file == selX    && rank == selY);
            bool isLegal   = legalSquares.count({file, rank}) > 0;

            /* Background colour — priority: selected > cursor > legal > normal */
            Color bg;
            if      (isSelected)  bg = Color::RGB( 50, 180,  50); /* bright green  */
            else if (isCursor && isLegal)
                                  bg = Color::RGB(200, 200,  50); /* yellow-green  */
            else if (isCursor)    bg = Color::RGB(220, 220,  50); /* yellow        */
            else if (isLegal && lightSq)
                                  bg = Color::RGB(130, 170, 100); /* legal light   */
            else if (isLegal)     bg = Color::RGB( 90, 130,  70); /* legal dark    */
            else if (lightSq)     bg = Color::RGB(240, 217, 181); /* normal light  */
            else                  bg = Color::RGB(181, 136,  99); /* normal dark   */

            /* Fill background */
            for (int dy = 0; dy < CELL_H; ++dy)
                for (int dx = 0; dx < CELL_W; ++dx)
                    c.DrawBlockLine(cx + dx, cy + dy, cx + dx, cy + dy, bg);

            /* Draw piece glyph */
            char p = pieceAt(fen, file, rank);
            if (p != '.')
            {
                Color fg = (p >= 'A' && p <= 'Z') ? Color::White : Color::RGB(30, 30, 30);
                c.DrawText(cx + CELL_W / 2 - 1, cy + CELL_H / 2 - 1,
                           pieceGlyph(p), [fg, bg](Pixel& px) {
                               px.foreground_color = fg;
                               px.background_color = bg;
                               px.bold = true;
                           });
            }
            /* Draw dot on legal empty squares */
            else if (isLegal)
            {
                c.DrawText(cx + CELL_W / 2 - 1, cy + CELL_H / 2 - 1, "o",
                           [bg](Pixel& px) {
                               px.foreground_color = Color::RGB(50, 50, 50);
                               px.background_color = bg;
                           });
            }
        }
    }

    /* Rank labels */
    for (int rank = 7; rank >= 0; --rank)
        c.DrawText(-2, (7 - rank) * CELL_H + CELL_H / 2 - 1,
                   std::to_string(rank + 1));

    /* File labels */
    for (int file = 0; file < 8; ++file)
        c.DrawText(file * CELL_W + CELL_W / 2 - 1, 8 * CELL_H,
                   std::string(1, char('a' + file)));
}

/* ------------------------------------------------------------------ */
/* Move list                                                            */
/* ------------------------------------------------------------------ */

static Elements buildMoveList(Game const& game)
{
    GameTree const& tree   = game.getTree();
    GameNode const* root   = tree.getRoot();
    GameNode const* cursor = tree.getCursor();

    Elements lines;
    GameNode const* node = root;
    int fullMove = 1;

    while (!node->children.empty())
    {
        GameNode const* wNode = node->children[0];
        std::ostringstream ws;
        ws << fullMove << ". " << wNode->move;
        std::string wStr = ws.str();
        while (wStr.size() < 12) wStr += ' ';

        std::string     bStr;
        GameNode const* bNode = nullptr;
        if (!wNode->children.empty())
        {
            bNode = wNode->children[0];
            std::ostringstream bs;
            bs << bNode->move;
            bStr = bs.str();
        }

        auto wElem = text(wStr);
        auto bElem = text(bStr);
        if (wNode == cursor) wElem = wElem | bold | color(Color::Yellow);
        if (bNode && bNode == cursor) bElem = bElem | bold | color(Color::Yellow);

        lines.push_back(hbox({wElem, bElem}));
        ++fullMove;
        node = bNode ? bNode : wNode;
    }

    return lines;
}

/* ------------------------------------------------------------------ */
/* Helpers                                                              */
/* ------------------------------------------------------------------ */

void TuiApp::clearSelection()
{
    selX = selY = -1;
    legalSquares.clear();
}

void TuiApp::computeLegalSquares()
{
    legalSquares.clear();
    if (selX < 0) return;

    bool whiteToMove = game.getColorToMove();
    auto moves = game.getBoard().getLegalMoves(whiteToMove);
    for (auto const& m : moves)
    {
        if (m.getXStart() == selX && m.getYStart() == selY)
            legalSquares.insert({m.getXEnd(), m.getYEnd()});
    }
}

void TuiApp::refreshFileList()
{
    fileList.clear();
    fileListSel = 0;
    try
    {
        if (!fs::exists(GAMES_DIR)) return;
        for (auto const& entry : fs::directory_iterator(GAMES_DIR))
        {
            if (entry.path().extension() == ".pgn")
                fileList.push_back(entry.path().filename().string());
        }
        std::sort(fileList.begin(), fileList.end());
    }
    catch (...) {}
}

/* ------------------------------------------------------------------ */
/* tryMove                                                              */
/* ------------------------------------------------------------------ */

void TuiApp::tryMove()
{
    if (selX < 0) return;

    bool ok = game.addMove(selX, selY, cursorX, cursorY, 0);
    if (ok)
        statusMsg = "Move applied.";
    else
        statusMsg = "Illegal move — try again.";

    clearSelection();
}

/* ------------------------------------------------------------------ */
/* confirmSave / confirmDbImport / confirmFilePick                      */
/* ------------------------------------------------------------------ */

void TuiApp::confirmSave()
{
    std::string path = GAMES_DIR;
    path += "/";
    path += saveBuf.empty() ? "game.pgn" : saveBuf;

    Parser parser;
    if (parser.saveGame(game, path, &gameMeta))
        statusMsg = "Saved: " + path;
    else
        statusMsg = "Failed to save: " + path;

    saveBuf.clear();
    saveDialogOpen = false;
}

void TuiApp::confirmDbImport()
{
    std::string path = GAMES_DIR;
    path += "/";
    path += dbInputBuf;

    if (!db)
    {
        try { db = std::make_unique<Database>(dbPath); }
        catch (...) { dbStatus = "Cannot open DB: " + dbPath; dbInputBuf.clear(); dbInputOpen = false; return; }
    }
    int n = db->importPGN(path);
    dbStatus = (n >= 0)
        ? "Imported " + std::to_string(n) + " game(s) from " + path
        : "Failed to import: " + path;
    dbInputBuf.clear();
    dbInputOpen = false;
}

void TuiApp::confirmFilePick()
{
    if (fileList.empty()) { filePickerOpen = false; return; }

    std::string path = GAMES_DIR;
    path += "/";
    path += fileList[fileListSel];

    GameMetadata meta;
    Game         newGame;
    Parser       parser;
    if (parser.loadGame(newGame, path, &meta))
    {
        game     = std::move(newGame);
        gameMeta = meta;
        clearSelection();
        statusMsg = "Loaded: " + path;
    }
    else
    {
        statusMsg = "Failed to load: " + path;
    }
    filePickerOpen = false;
}

/* ------------------------------------------------------------------ */
/* Event handling — Board screen                                        */
/* ------------------------------------------------------------------ */

bool TuiApp::handleBoardEvent(Event const& ev)
{
    /* ---- File picker ---- */
    if (filePickerOpen)
    {
        if (ev == Event::Return)  { confirmFilePick(); return true; }
        if (ev == Event::Escape)  { filePickerOpen = false; return true; }
        if (ev == Event::ArrowUp   && fileListSel > 0)
            { --fileListSel; return true; }
        if (ev == Event::ArrowDown && fileListSel < static_cast<int>(fileList.size()) - 1)
            { ++fileListSel; return true; }
        return true; /* swallow other keys */
    }

    /* ---- Save dialog ---- */
    if (saveDialogOpen)
    {
        if (ev == Event::Return)   { confirmSave(); return true; }
        if (ev == Event::Escape)   { saveDialogOpen = false; saveBuf.clear(); return true; }
        if (ev == Event::Backspace){ if (!saveBuf.empty()) saveBuf.pop_back(); return true; }
        if (ev.is_character())     { saveBuf += ev.character(); return true; }
        return true;
    }

    /* ---- Board navigation ---- */
    if (ev == Event::ArrowUp    && cursorY < 7) { ++cursorY; return true; }
    if (ev == Event::ArrowDown  && cursorY > 0) { --cursorY; return true; }
    if (ev == Event::ArrowLeft  && cursorX > 0) { --cursorX; return true; }
    if (ev == Event::ArrowRight && cursorX < 7) { ++cursorX; return true; }

    /* ---- Select / move ---- */
    if (ev == Event::Return)
    {
        if (selX < 0)
        {
            /* Select a piece — enforce turn order */
            std::string fen = game.getFEN();
            char p = pieceAt(fen, cursorX, cursorY);
            if (p == '.') { statusMsg = "Empty square — pick a piece."; return true; }

            bool whiteToMove  = game.getColorToMove();
            bool pieceIsWhite = (p >= 'A' && p <= 'Z');
            if (pieceIsWhite != whiteToMove)
            {
                statusMsg = whiteToMove ? "It is White's turn." : "It is Black's turn.";
                return true;
            }

            selX = cursorX; selY = cursorY;
            computeLegalSquares();
            statusMsg = "Piece selected — move cursor to destination and press Enter.";
        }
        else
        {
            tryMove();
        }
        return true;
    }

    if (ev == Event::Escape) { clearSelection(); statusMsg = "Deselected."; return true; }

    /* ---- Move tree navigation ---- */
    if (ev == Event::Character('<') || ev == Event::Character(','))
    {
        game.back(); clearSelection(); return true;
    }
    if (ev == Event::Character('>') || ev == Event::Character('.'))
    {
        game.forward(); clearSelection(); return true;
    }

    /* ---- Load (file picker) ---- */
    if (ev == Event::Character('l') || ev == Event::Character('L'))
    {
        refreshFileList();
        filePickerOpen = true;
        return true;
    }

    /* ---- Save dialog ---- */
    if (ev == Event::Character('s') || ev == Event::Character('S'))
    {
        saveDialogOpen = true;
        saveBuf.clear();
        return true;
    }

    return false;
}

/* ------------------------------------------------------------------ */
/* Event handling — Database screen                                     */
/* ------------------------------------------------------------------ */

bool TuiApp::handleDatabaseEvent(Event const& ev)
{
    if (dbInputOpen)
    {
        if (ev == Event::Return)   { confirmDbImport(); return true; }
        if (ev == Event::Escape)   { dbInputOpen = false; dbInputBuf.clear(); return true; }
        if (ev == Event::Backspace){ if (!dbInputBuf.empty()) dbInputBuf.pop_back(); return true; }
        if (ev.is_character())     { dbInputBuf += ev.character(); return true; }
        return true;
    }
    if (ev == Event::Character('i') || ev == Event::Character('I'))
    {
        dbInputOpen = true; dbInputBuf.clear(); return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main render + loop                                                   */
/* ------------------------------------------------------------------ */

void TuiApp::run()
{
    auto ftxuiScreen = ScreenInteractive::Fullscreen();

    auto component = Renderer([&]() -> Element
    {
        /* Tab bar */
        auto tab1 = text(" [1] Board    ");
        auto tab2 = text(" [2] Database ");
        auto tab3 = text(" [3] Analysis ");
        if (screen == TuiScreen::Board)    tab1 = tab1 | bold | color(Color::Yellow);
        if (screen == TuiScreen::Database) tab2 = tab2 | bold | color(Color::Yellow);
        if (screen == TuiScreen::Analysis) tab3 = tab3 | bold | color(Color::Yellow);
        auto tabBar = hbox({tab1, tab2, tab3, filler(), text(" [Q] Quit ")}) | border;

        /* ================================================================ */
        /* Board screen                                                       */
        /* ================================================================ */
        if (screen == TuiScreen::Board)
        {
            int    canvasW = 8 * CELL_W + 4;
            int    canvasH = 8 * CELL_H + 2;
            Canvas boardObj(canvasW, canvasH);
            renderBoardPanel(boardObj);
            auto boardElem = canvas(std::move(boardObj));

            /* Move list */
            auto moveElems = buildMoveList(game);
            Element moveList = moveElems.empty()
                ? text("(no moves yet)") | color(Color::GrayDark)
                : vbox(moveElems);

            bool isWhite = game.getColorToMove();
            auto rightPanel = vbox({
                text("Move list") | bold | underlined,
                separator(),
                moveList | yframe,
                filler(),
                separator(),
                text(isWhite ? "White to move" : "Black to move") | bold
                    | color(isWhite ? Color::White : Color::GrayDark),
                separator(),
                text("< > tree   L load   S save") | color(Color::GrayDark),
                text("Enter select/move   Esc deselect") | color(Color::GrayDark),
            }) | border | size(WIDTH, GREATER_THAN, 30);

            std::string fen = game.getFEN();
            auto statusBar = vbox({
                text(fen) | color(Color::GrayDark),
                text(statusMsg) | color(Color::Cyan),
            }) | border;

            auto mainArea = hbox({boardElem | border, rightPanel});

            /* ---- File picker overlay ---- */
            if (filePickerOpen)
            {
                Elements items;
                if (fileList.empty())
                    items.push_back(text("(no .pgn files in " + std::string(GAMES_DIR) + ")")
                                    | color(Color::GrayDark));
                else
                    for (int i = 0; i < static_cast<int>(fileList.size()); ++i)
                    {
                        auto row = text((i == fileListSel ? "> " : "  ") + fileList[i]);
                        if (i == fileListSel) row = row | bold | color(Color::Yellow);
                        items.push_back(row);
                    }

                auto dialog = vbox({
                    text("Load PGN from " + std::string(GAMES_DIR) + "/") | bold,
                    separator(),
                    vbox(items),
                    separator(),
                    text("↑↓ navigate   Enter = load   Esc = cancel") | color(Color::GrayDark),
                }) | border | size(WIDTH, EQUAL, 50);

                return vbox({tabBar, mainArea, statusBar, dialog | center});
            }

            /* ---- Save dialog overlay ---- */
            if (saveDialogOpen)
            {
                auto dialog = vbox({
                    text("Save PGN to " + std::string(GAMES_DIR) + "/") | bold,
                    separator(),
                    hbox({text("> "), text(saveBuf.empty() ? "game.pgn" : saveBuf)
                          | (saveBuf.empty() ? color(Color::GrayDark) : color(Color::White)),
                          text("_") | color(Color::Yellow)}),
                    separator(),
                    text("Enter = confirm   Esc = cancel") | color(Color::GrayDark),
                }) | border | size(WIDTH, EQUAL, 50);

                return vbox({tabBar, mainArea, statusBar, dialog | center});
            }

            return vbox({tabBar, mainArea, statusBar});
        }

        /* ================================================================ */
        /* Database screen                                                    */
        /* ================================================================ */
        if (screen == TuiScreen::Database)
        {
            auto content = vbox({
                text("Chess Database") | bold | underlined,
                separator(),
                text("Database file: " + dbPath) | color(Color::GrayDark),
                separator(),
                text("Press I to import a PGN file from " + std::string(GAMES_DIR) + "/"),
                separator(),
                text(dbStatus.empty() ? "(no import yet)" : dbStatus)
                    | color(dbStatus.empty() ? Color::GrayDark : Color::Green),
            }) | border;

            if (dbInputOpen)
            {
                auto dialog = vbox({
                    text("Filename in " + std::string(GAMES_DIR) + "/") | bold,
                    separator(),
                    hbox({text("> "), text(dbInputBuf), text("_") | color(Color::Yellow)}),
                    separator(),
                    text("Enter = confirm   Esc = cancel") | color(Color::GrayDark),
                }) | border | size(WIDTH, EQUAL, 60);
                return vbox({tabBar, content, filler(), dialog | center});
            }
            return vbox({tabBar, content, filler()});
        }

        /* ================================================================ */
        /* Analysis screen                                                    */
        /* ================================================================ */
        return vbox({
            tabBar,
            vbox({
                text("Stockfish Analysis") | bold | underlined,
                separator(),
                text("UCIEngine and Statistics are in core/analysis/."),
                text(""),
                text("  UCIEngine engine;"),
                text("  engine.connect(\"/usr/bin/stockfish\");"),
                text("  engine.setPosition(game.getFEN());"),
                text("  EngineResult r = engine.analyze(20);"),
                text("  // r.bestMove, r.score, r.pvLine"),
                text(""),
                text("  Database db(\"chess.db\");"),
                text("  Statistics stats(db, engine);"),
                text("  stats.analyzeGame(gameId);"),
                text("  auto blunders = stats.detectBlunders(gameId);"),
            }) | border,
            filler(),
        });
    });

    component = CatchEvent(component, [&](Event ev) -> bool
    {
        if (ev == Event::Character('1')) { screen = TuiScreen::Board;    return true; }
        if (ev == Event::Character('2')) { screen = TuiScreen::Database; return true; }
        if (ev == Event::Character('3')) { screen = TuiScreen::Analysis; return true; }
        if ((ev == Event::Character('q') || ev == Event::Character('Q'))
            && !filePickerOpen && !saveDialogOpen && !dbInputOpen)
        {
            ftxuiScreen.ExitLoopClosure()();
            return true;
        }
        if (screen == TuiScreen::Board)    return handleBoardEvent(ev);
        if (screen == TuiScreen::Database) return handleDatabaseEvent(ev);
        return false;
    });

    ftxuiScreen.Loop(component);
}
