/**
 * @file  tui_app.hpp
 * @brief FTXUI terminal UI application
 */

#ifndef _TUI_APP_HPP_
#define _TUI_APP_HPP_

#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "game.hpp"
#include "parser.hpp"
#include "database.hpp"

namespace ftxui { class Canvas; class Event; }

/** Default directory for PGN files */
static constexpr const char* GAMES_DIR = "data/games";

/**
 * @brief Active screen tab
 */
enum class TuiScreen
{
    Board    = 0, /**< Interactive board + move navigation */
    Database = 1, /**< PGN import and game search */
    Analysis = 2, /**< Analysis instructions */
};

/**
 * @brief Terminal chess UI built with FTXUI
 *
 * Screens:
 *  - Board (Tab 1): interactive board, piece selection, move tree navigation
 *  - Database (Tab 2): import PGN files into a local SQLite database
 *  - Analysis (Tab 3): instructions for Stockfish integration
 *
 * Keyboard shortcuts (Board screen):
 *  Arrow keys  — move board cursor
 *  Enter       — select piece / confirm move
 *  Escape      — deselect piece
 *  < >         — navigate move tree backward/forward
 *  L           — open load-PGN file picker (data/games/)
 *  S           — open save-PGN dialog
 *  1/2/3       — switch screen
 *  Q           — quit
 */
class TuiApp
{
public:
    TuiApp();

    /**
     * @brief Start the full-screen terminal UI (blocks until the user quits)
     */
    void run();

private:
    /* ---- Game state ---- */
    Game         game;
    GameMetadata gameMeta;
    std::string  statusMsg;

    /* ---- Board screen ---- */
    int  cursorX = 4; /**< File (0=a..7=h) of the board cursor */
    int  cursorY = 0; /**< Rank (0=rank1..7=rank8) of the board cursor */
    int  selX    = -1; /**< Selected piece file (-1 = none) */
    int  selY    = -1; /**< Selected piece rank (-1 = none) */

    /** Destination squares for the selected piece (filled on selection) */
    std::set<std::pair<int,int>> legalSquares;

    /* ---- File picker (Load dialog) ---- */
    bool                     filePickerOpen = false;
    std::vector<std::string> fileList;       /**< Filenames in GAMES_DIR */
    int                      fileListSel = 0; /**< Highlighted file index */

    /* ---- Save dialog ---- */
    bool        saveDialogOpen = false;
    std::string saveBuf;

    /* ---- Screen ---- */
    TuiScreen screen = TuiScreen::Board;

    /* ---- Database screen ---- */
    std::string            dbPath   = "chess.db";
    std::string            dbStatus;
    bool                   dbInputOpen = false;
    std::string            dbInputBuf;
    std::unique_ptr<Database> db;

    /* ---- Rendering helpers ---- */

    /**
     * @brief Return the piece character from a FEN string
     * @param fen Full FEN string
     * @param x   File (0=a..7=h)
     * @param y   Rank (0=rank1..7=rank8)
     * @return Piece char (uppercase=white, lowercase=black) or '.' for empty
     */
    static char pieceAt(std::string const& fen, int x, int y);

    /**
     * @brief Render the 8x8 board panel into a Canvas
     */
    void renderBoardPanel(class ftxui::Canvas& c) const;

    /**
     * @brief Scan GAMES_DIR and populate fileList
     */
    void refreshFileList();

    /* ---- Event handlers ---- */

    /** Handle keyboard on the Board screen; returns true if consumed */
    bool handleBoardEvent(class ftxui::Event const& ev);

    /** Handle keyboard on the Database screen; returns true if consumed */
    bool handleDatabaseEvent(class ftxui::Event const& ev);

    /** Confirm save dialog */
    void confirmSave();

    /** Confirm DB import dialog */
    void confirmDbImport();

    /** Load the file currently highlighted in the file picker */
    void confirmFilePick();

    /** Attempt to move the selected piece to (cursorX, cursorY) */
    void tryMove();

    /** Clear piece selection and legal-move highlights */
    void clearSelection();

    /** Recompute legalSquares for the piece at (selX, selY) */
    void computeLegalSquares();
};

#endif /* _TUI_APP_HPP_ */
