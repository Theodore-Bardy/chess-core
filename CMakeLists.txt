# @file  CMakeLists.txt
# @brief CMakeLists file of the repository

cmake_minimum_required(VERSION 3.19)

include(FetchContent)

# SQLiteCpp — MIT license, cross-platform SQLite C++ wrapper
FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG        3.3.1
)
set(SQLITECPP_RUN_CPPLINT  OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SQLiteCpp)

# C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Set project's name
set(PROJECT_BASE_NAME chess_core)

option(UNIT_TESTS "Build unit tests" OFF)

if(UNIT_TESTS)
    message("Build unit tests")
    include(${CMAKE_CURRENT_LIST_DIR}/tests/CMakeLists.txt)
else()
    project(${PROJECT_BASE_NAME} LANGUAGES C CXX)

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug)
    endif()

    # Executable name — no hardcoded .elf extension; CMake adds .exe on Windows automatically
    set(APP_EXECUTABLE_NAME ${PROJECT_BASE_NAME})
    message("Executable name: ${APP_EXECUTABLE_NAME}")
    add_executable(${APP_EXECUTABLE_NAME})

    # Compile options — cross-platform via generator expressions
    target_compile_options(${APP_EXECUTABLE_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-O1>
        $<$<CXX_COMPILER_ID:MSVC>:/O1>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU,Clang>>:-g>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Zi>
    )

    # Include sub-directories
    include(${CMAKE_CURRENT_LIST_DIR}/app/CMakeLists.txt)
    include(${CMAKE_CURRENT_LIST_DIR}/core/CMakeLists.txt)

    # Link SQLiteCpp
    target_link_libraries(${APP_EXECUTABLE_NAME} PRIVATE SQLiteCpp)
endif()
