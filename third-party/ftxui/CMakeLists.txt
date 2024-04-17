# @file  CMakeLists.txt
# @brief CMakeLists file for FTXUI library

# Include FTXUI library
set(GIT_REPO_URL "https://github.com/ArthurSonzogni/FTXUI.git")
set(GIT_TAG_NAME "v5.0.0")

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY  ${GIT_REPO_URL}
    GIT_TAG         ${GIT_TAG_NAME}
)
FetchContent_MakeAvailable(ftxui)

add_executable(ftxui-starter ${CMAKE_CURRENT_LIST_DIR}/src/ftxui_demo.cpp)
target_link_libraries(ftxui-starter
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component # Not needed for this example.
)

# Pieces sources
file(GLOB_RECURSE SOURCES_TEMP
    ${CMAKE_SOURCE_DIR}/core/game/src/*.cpp
    ${CMAKE_SOURCE_DIR}/core/pieces/src/*.cpp
)
target_sources(ftxui-starter PRIVATE ${SOURCES_TEMP})

# Pieces includes
target_include_directories(ftxui-starter PRIVATE ${CMAKE_SOURCE_DIR}/core/game/include)
target_include_directories(ftxui-starter PRIVATE ${CMAKE_SOURCE_DIR}/core/pieces/include)
