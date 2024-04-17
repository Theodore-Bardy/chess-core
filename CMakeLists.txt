# @file  CMakeLists.txt
# @brief CMakeLists file of the repository

cmake_minimum_required(VERSION 3.19)

# CMake configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# C11 according to Code Style
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
    # Declare the executable first, so that we can add flags and sources later on
    set(APP_EXECUTABLE_NAME ${PROJECT_BASE_NAME}.elf)
    message("Executable name: ${APP_EXECUTABLE_NAME}")
    add_executable(${APP_EXECUTABLE_NAME})

    # Specify GCC toolchain
    set(CMAKE_C_COMPILER /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)

    # Define compile options
    target_compile_options(${APP_EXECUTABLE_NAME} PRIVATE -O1)
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_compile_options(${APP_EXECUTABLE_NAME} PRIVATE -g)
    endif()

    # Set project
    project(${PROJECT_BASE_NAME} LANGUAGES)

    # Include sub-directories
    include(${CMAKE_CURRENT_LIST_DIR}/app/CMakeLists.txt)
    include(${CMAKE_CURRENT_LIST_DIR}/core/CMakeLists.txt)
    include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)

    # Enable used languages. This must be done after the toolchain has been defined by the BSP.
    enable_language(C CXX ASM)
endif(UNIT_TESTS)
