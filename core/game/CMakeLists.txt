# @file  CMakeLists.txt
# @brief CMakeLists file for the game core

# Game core sources
file(GLOB_RECURSE SOURCES_TEMP ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
target_sources(${APP_EXECUTABLE_NAME} PRIVATE ${SOURCES_TEMP})

# Game core includes
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
