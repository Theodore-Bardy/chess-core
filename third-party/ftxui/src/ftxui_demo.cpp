#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

#include "ftxui/component/component.hpp"          // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/event.hpp"              // for Event
#include "ftxui/component/mouse.hpp"              // for Mouse
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/canvas.hpp"                   // for Canvas
#include "ftxui/screen/color.hpp"                 // for Color, Color::Red, Color::Blue, Color::Green, ftxui

#include "board.hpp"
#include "piece.hpp"

int
main(void)
{
    using namespace ftxui;

    Board gameBoard;
    gameBoard.startUp();

    // Add some separator to decorate the whole component:
    auto component_renderer = Renderer([&] {
        return vbox({
                   text("Player 1") | border,
                   text(gameBoard.printWhiteDeathPieces()),
                   text(gameBoard.printBoardLine(7)),
                   text(gameBoard.printBoardLine(6)),
                   text(gameBoard.printBoardLine(5)),
                   text(gameBoard.printBoardLine(4)),
                   text(gameBoard.printBoardLine(3)),
                   text(gameBoard.printBoardLine(2)),
                   text(gameBoard.printBoardLine(1)),
                   text(gameBoard.printBoardLine(0)),
                   text(gameBoard.printDashLine()),
                   text(gameBoard.printXLine()),
                   text(gameBoard.printBlackDeathPieces()),
                   text("Player 2") | border,
               })
               | border;
    });

    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(component_renderer);

    return EXIT_SUCCESS;
}
