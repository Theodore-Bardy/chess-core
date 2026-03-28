# @file  CMakeLists.txt
# @brief CMakeLists file for the FTXUI terminal UI

# TUI sources
file(GLOB_RECURSE SOURCES_TEMP ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
target_sources(${APP_EXECUTABLE_NAME} PRIVATE ${SOURCES_TEMP})

# TUI includes
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
