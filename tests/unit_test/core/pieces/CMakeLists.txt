# @file  CMakeLists.txt
# @brief CMakeLists file for the pieces tests

include(AddGoogleTest)
if (UNIX)
    include(CodeCoverage)
endif (UNIX)

# Compile tests
add_google_test(bishop_test
    SOURCES ${CMAKE_SOURCE_DIR}/core/pieces/src/bishop.cpp ${CMAKE_SOURCE_DIR}/core/pieces/src/piece.cpp ${CMAKE_CURRENT_LIST_DIR}/src/bishop_test.cpp
    COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS} -g -O0 --coverage
    LINK_LIBRARIES GTest::gtest_main gcov
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/core/game/include ${CMAKE_SOURCE_DIR}/core/pieces/include
)

add_google_test(rook_test
    SOURCES ${CMAKE_SOURCE_DIR}/core/pieces/src/rook.cpp ${CMAKE_SOURCE_DIR}/core/pieces/src/piece.cpp ${CMAKE_CURRENT_LIST_DIR}/src/rook_test.cpp
    COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS} -g -O0 --coverage
    LINK_LIBRARIES GTest::gtest_main gcov
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/core/game/include ${CMAKE_SOURCE_DIR}/core/pieces/include
)

# Add tests coverage
if (UNIX)
    add_test_unit_coverage(bishop_test_coverage ctest bishop_test)
    add_test_unit_coverage(rook_test_coverage ctest rook_test)
endif (UNIX)
