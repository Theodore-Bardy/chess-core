# Chess Database & Analysis Tool — Project Guide

## Project Overview

A personal chess database and analysis tool running locally on **Linux and Windows**.

**Goals:**
- Chess engine: rules-complete board, move generation, game variants (PGN branches)
- Database: import/search/store games (SQLite + PGN)
- Analysis: Stockfish integration, blunder detection, opening stats, performance trends
- UI: CLI (done) → TUI (FTXUI) → GUI (Qt 6)

---

## Architecture

```
chess-core/
├── app/                  # Entry point (CLI, later TUI/GUI selector)
├── core/
│   ├── game/             # Board, Move, Square, Game, GameTree (planned)
│   ├── pieces/           # Piece hierarchy (King/Queen/Rook/Bishop/Knight/Pawn)
│   ├── parser/           # PGN read/write
│   ├── database/         # SQLite layer (planned Phase 4)
│   └── analysis/         # Stockfish UCI + statistics (planned Phase 5)
├── ui/
│   ├── tui/              # FTXUI terminal UI (planned Phase 6)
│   └── gui/              # Qt 6 GUI (planned Phase 7)
├── tests/                # Google Test unit tests
└── docs/uml/             # PlantUML class diagrams (kept in sync with code)
```

---

## Technical Decisions

| Topic | Decision | Reason |
|---|---|---|
| Language | C++ 17 | Existing codebase; performance |
| Build | CMake (≥ 3.19), no hardcoded toolchain | Cross-platform (Linux + Windows) |
| Tests | Google Test 1.14.0 via FetchContent | Already in repo |
| Coverage | Optional (`-DCODE_COVERAGE=ON`), requires lcov | Not available on all machines |
| Database | SQLiteCpp (MIT) via FetchContent | Cross-platform SQLite wrapper |
| TUI | FTXUI (MIT) via FetchContent | Already in README TODO |
| GUI | Qt 6 (LGPL) | Mature, cross-platform, C++ native |
| Subprocess | Boost.Process (BSL-1.0) | Cross-platform fork/exec replacement |
| Analysis engine | Stockfish via UCI over stdin/stdout | GPL-3 subprocess — does not infect binary |
| UML | PlantUML `.puml` in `docs/uml/` | Text-based, git-friendly |
| Style | Flag-based moves (`MOVE_FLAG_*`), polymorphic pieces, no RAII wrappers | Match existing code |

### Move Flags (bitmask in `move.hpp`)
```
MOVE_FLAG_TAKE         0x01  — capture
MOVE_FLAG_KING_CASTLE  0x02  — kingside castling
MOVE_FLAG_QUEEN_CASTLE 0x04  — queenside castling
MOVE_FLAG_CHECK        0x08  — move gives check
MOVE_FLAG_CHECK_MATE   0x10  — move gives checkmate
MOVE_FLAG_EN_PASSANT   0x20  — en passant capture
MOVE_FLAG_PROMOTION    0x40  — pawn promotion
```

---

## What Has Been Done

### Phase 0 — Cross-Platform Build & UML Foundation ✅

- **CMakeLists.txt**: Removed hardcoded `/usr/bin/gcc` compiler paths. Added CMake generator expressions for compile flags (`$<$<CXX_COMPILER_ID:GNU,Clang>:-O1>` etc.). Executable renamed from `chess_core.elf` to `chess_core` (CMake adds `.exe` on Windows automatically).
- **Test coverage**: Made `lcov` optional via `-DCODE_COVERAGE=ON` so tests can run without it.
- **UML diagrams**: Created `docs/uml/` with 6 PlantUML files:
  - `00_overview.puml` — component overview
  - `01_pieces.puml` — Piece hierarchy
  - `02_game_engine.puml` — Board, Move, Game, GameTree (current + planned)
  - `03_parser.puml` — Parser, GameMetadata (current + planned)
  - `04_database.puml` — Database layer (planned)
  - `05_analysis.puml` — UCIEngine, Statistics (planned)

### Phase 1 — Chess Engine Completion ✅

#### 1.1 ✅ Move Legality Validation
- Fixed **`checkWayOnMove`** in `piece.cpp`: direction was inverted and loop used `&&` instead of `||`, causing sliding pieces (rook/bishop/queen) to skip path obstruction checks.
- Fixed **`isSquareAttacked`** in `board.cpp`: was passing `flags=0` so pawn diagonal attacks were never detected. Now passes `MOVE_FLAG_TAKE`.
- Fixed **Board copy constructor**: named pointers (`whiteKing`, `whiteBishops[]`, etc.) were uninitialized after copy. Added `setupNamedPointers()` private method that rebuilds them from the piece vectors.
- Added **`isInCheck(bool color)`**: scans piece vector for the king, then calls `isSquareAttacked`.
- Added **`isMoveLegal(Piece*, xEnd, yEnd, flags)`**: copies the board, applies the move on the copy, checks `isInCheck`. Integrated into `Board::checkMove` (PGN path) and `Game::addMove` (interactive path).

#### 1.2 ✅ Full Legal Move Generation
- Added **`getLegalMoves(bool color)`** to `Board`: iterates all alive pieces, tries all 64 destination squares, applies board context to `isAbleToMove`, filters by `isMoveLegal`.

#### 1.3 ✅ En Passant
- Added `MOVE_FLAG_EN_PASSANT` flag.
- Added `enPassantX`/`enPassantY` fields to `Board` (initialized to -1 = none).
- `movePiece` now sets the en passant target after a pawn double-push, clears it on all other moves.
- `movePiece` handles en passant capture: removes the captured pawn from its actual square (same rank as moving pawn, same file as destination).
- `getLegalMoves` generates en passant moves for pawns when `enPassantX != -1`.

#### 1.4 ✅ Pawn Promotion
- Added `MOVE_FLAG_PROMOTION` flag.
- Added `promotionPiece : SquarePieceValue` field to `Move` (default `Empty`).
- `movePiece` now accepts `promotionPiece` parameter (default: `QueenValue`). When `MOVE_FLAG_PROMOTION` is set, the pawn is marked dead and a new piece of the chosen type is created and added to the piece vector.
- `getLegalMoves` generates 4 promotion moves (Q, R, B, N) when a pawn reaches the back rank.

#### 1.5 ✅ Checkmate & Stalemate Detection
- Added `GameResult` enum to `board.hpp` (InProgress, WhiteWins, BlackWins, DrawStalemate, DrawFiftyMove, DrawThreefold).
- Added `getGameResult(bool colorToMove)` to `Board`: calls `getLegalMoves()` and `isInCheck()` to determine the result.

#### 1.6 ✅ Fifty-Move Rule & Threefold Repetition
- Added `halfMoveClock : int` to `Board` (resets on capture or pawn move, draws at 100).
- Added `positionHistory : map<string, int>` to `Board`.
- Added `computeHash(bool) : string` to `Board` (piece layout + side to move + en passant file).
- Added `updateDrawTracking(bool, bool, bool)` called from `movePiece` after each move.

#### 1.7 ✅ Fix Castling Through Check
- Removed stale `!isCheck` field gate from `King::castling()` and `King::isAbleToMove()` (field was never updated so always allowed castling even in check).
- Added `!r->getHasMoved()` check (rook eligibility) and `!isInCheck(piece->getColor())` (king not in check) to both castling blocks in `Board::movePiece`.
- Updated `isMoveLegal` to check `movePiece` return value: returns false if the move could not be applied (covers the castling-through-check case).
- Added castling move generation to `getLegalMoves`: tries both `MOVE_FLAG_KING_CASTLE` and `MOVE_FLAG_QUEEN_CASTLE` for an unmoved king via `isMoveLegal`.
- Added `getHasMoved()` accessor to both `King` and `Rook`.

---

## What Needs to Be Done

### Phase 2 — Game Tree / Variants ✅

- Implemented `GameNode` (move + Board snapshot + comment + parent/children vector) in `core/game/include/game_tree.hpp` and `core/game/src/game_tree.cpp`.
- Implemented `GameTree`: root (starting position), cursor, `addMove`, `addVariation`, `startVariation`/`endVariation` (for PGN parser), `forward(int)`, `back()`, `getDepth()`.
- Replaced `vector<Move*>` in `Game` with `GameTree`. `Game` now tracks `colorToMove` and `fullMoveNumber`. Navigation methods `forward()` / `back()` update both the tree cursor and `gameBoard` from the node's Board snapshot.
- Added `Board::getFEN(bool colorToMove, int fullMoveNumber)` — generates standard FEN string (piece placement, active color, castling rights, en passant, halfmove clock, fullmove number).
- Fixed `Move::print()` bug: queen-side castling branch was checking `MOVE_FLAG_KING_CASTLE` instead of `MOVE_FLAG_QUEEN_CASTLE`.
- `Game::getFEN()` convenience method delegates to `gameBoard.getFEN(colorToMove, fullMoveNumber)`.
- `Game::getTree()` exposes the tree read-only for the PGN generator (Phase 3).

### Phase 3 — Full PGN Support ✅

- Rewrote `core/parser/src/parser.cpp` with a full tokenizer → recursive parser pipeline.
- `loadGame()`: parses `[Header "value"]` tags into `GameMetadata`; tokenizes the move text into `TOK_MOVE_NUM`, `TOK_MOVE`, `TOK_LPAREN`, `TOK_RPAREN`, `TOK_COMMENT`, `TOK_RESULT`; `applyTokens()` recurses into `(...)` variations via `game.startVariation()` / `game.endVariation()`; handles `e8=Q` promotion, `+`/`#` check flags, full disambiguation (file, rank, full-square).
- `saveGame()`: `moveToSAN()` produces standard algebraic notation (O-O/O-O-O, `exd5`, `e8=Q`, `Nxf3`, `Raf1`); `writeNode()` walks `GameTree` recursively, emitting variations in `( )`, move numbers, and `{comments}`.
- Verified: `gameExample.pgn` → 67 moves played, correct final FEN `2R5/p7/1p3k2/8/5n2/8/PP2BK2/6R1 b - - 0 34`, round-trip PGN output preserves all headers, comments, and SAN notation.

### Phase 4 — SQLite Database Layer ✅

- Added **SQLiteCpp 3.3.1** (MIT) via CMake FetchContent (root `CMakeLists.txt`).
- Created `core/database/` module: `database.hpp` / `database.cpp` + `CMakeLists.txt`.
- `Database` class:
  - `importPGN(path)` — parses PGN via `Parser::loadGame()`, inserts into `games` + `positions` tables, walks main-line game tree for FEN snapshots.
  - `searchGames(GameFilter)` — filter by white/black/result/dateFrom/dateTo/eco/minElo/maxElo, substring match on player names.
  - `getGame(id)` — retrieves full `pgn_text` + metadata.
  - `getGamesByPosition(fen)` — indexed lookup on `positions.fen`.
  - `saveAnalysis(gameId, moveNum, eval)` — stores engine eval (used by Phase 5).
  - `getPlayerStats(name)` — aggregates win/draw/loss counts.
- Schema: `players`, `games`, `positions` tables with FKs, WAL mode, `PRAGMA foreign_keys = ON`.

### Phase 5 — Stockfish Analysis Integration ✅

- No external subprocess dependency: implemented a thin `Process` class with `#ifdef _WIN32` / POSIX guards (fork/exec/pipe/select on Linux; CreateProcess/CreatePipe/PeekNamedPipe on Windows).
- Created `core/analysis/` module: `process.hpp/cpp`, `uci_engine.hpp/cpp`, `statistics.hpp/cpp`.
- `UCIEngine`: `connect()` spawns engine and completes UCI handshake (`uci`/`uciok`, `isready`/`readyok`); `setPosition(fen)` sends `position fen`; `analyze(depth)` sends `go depth N`, parses `info` lines for score/depth/PV, waits for `bestmove`; `shutdown()` sends `quit`.
- `Statistics`:
  - `analyzeGame(gameId, depth)` — re-parses stored PGN, evaluates every main-line position via Stockfish, stores result via `Database::saveAnalysis()`.
  - `detectBlunders(gameId)` — reads stored evals via `Database::getEvals()`, computes eval drop per move from side-to-move perspective; flags > 0.5 pawns as mistake, > 2.0 as blunder.
  - `getOpeningStats(eco)` — aggregates win/draw/loss for an ECO prefix.
  - `getPositionFrequency(fen)` — counts games reaching a given FEN.
  - `getPerformanceTrend(player, dateFrom, dateTo)` — buckets games by YYYY.MM, computes win rate per month.
- Added `Database::getEvals(gameId)` to support blunder detection (returns eval_score rows ordered by move_num).

### Phase 6 — TUI (FTXUI)
- Add **FTXUI** (MIT) via CMake FetchContent.
- Create `ui/tui/` with screens: Board, Game tree, Database browser, Analysis panel.
- Replace the raw `cin`/`cout` loop in `app/src/main.cpp`.

### Phase 7 — Qt 6 GUI
- Qt 6 (LGPL), system install or vcpkg.
- `QChessBoard` widget, `MoveListWidget`, `AnalysisPanel`, `DatabaseBrowser`.
- Detailed design deferred until Phase 6 is complete.

---

## Building

```bash
# App (Linux/Windows)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Tests (no coverage)
cmake -S . -B build_tests -DUNIT_TESTS=ON
cmake --build build_tests
ctest --test-dir build_tests --output-on-failure

# Tests with coverage (requires lcov)
cmake -S . -B build_tests -DUNIT_TESTS=ON -DCODE_COVERAGE=ON
cmake --build build_tests
ctest --test-dir build_tests --output-on-failure
```

---

## UML Update Policy

Every code change that affects class interfaces must update the relevant `docs/uml/*.puml` file. The overview (`00_overview.puml`) is updated at the end of each phase.
