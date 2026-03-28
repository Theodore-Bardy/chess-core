/**
 * @file  main.cpp
 * @brief Application entry point — launches the FTXUI terminal UI
 */

#include "tui_app.hpp"

int main(int /*argc*/, char const* /*argv*/[])
{
    TuiApp app;
    app.run();
    return 0;
}
