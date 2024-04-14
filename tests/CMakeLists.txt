# @file  CMakeLists.txt
# @brief CMakeLists file for tests

# Include GoogleTest Framework
set(GIT_REPO_URL "https://github.com/google/googletest.git")
set(GIT_TAG_NAME "v1.14.0")

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY  ${GIT_REPO_URL}
    GIT_TAG         ${GIT_TAG_NAME}
    GIT_SUBMODULES  ""
)

# [For Windows]: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Set project for unit tests
project(${PROJECT_BASE_NAME} LANGUAGES CXX)

# Add CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake-modules)

# Add all tests
include(${CMAKE_CURRENT_LIST_DIR}/unit_test/CMakeLists.txt)
