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

add_google_test(king_test
    SOURCES ${CMAKE_SOURCE_DIR}/core/pieces/src/king.cpp ${CMAKE_SOURCE_DIR}/core/pieces/src/rook.cpp ${CMAKE_SOURCE_DIR}/core/pieces/src/piece.cpp ${CMAKE_CURRENT_LIST_DIR}/src/king_test.cpp
    COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS} -g -O0 --coverage
    LINK_LIBRARIES GTest::gtest_main gcov
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/core/game/include ${CMAKE_SOURCE_DIR}/core/pieces/include
)

add_google_test(knight_test
    SOURCES ${CMAKE_SOURCE_DIR}/core/pieces/src/knight.cpp ${CMAKE_SOURCE_DIR}/core/pieces/src/piece.cpp ${CMAKE_CURRENT_LIST_DIR}/src/knight_test.cpp
    COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS} -g -O0 --coverage
    LINK_LIBRARIES GTest::gtest_main gcov
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/core/game/include ${CMAKE_SOURCE_DIR}/core/pieces/include
)

add_google_test(pawn_test
    SOURCES ${CMAKE_SOURCE_DIR}/core/pieces/src/pawn.cpp ${CMAKE_SOURCE_DIR}/core/pieces/src/piece.cpp ${CMAKE_CURRENT_LIST_DIR}/src/pawn_test.cpp
    COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS} -g -O0 --coverage
    LINK_LIBRARIES GTest::gtest_main gcov
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/core/game/include ${CMAKE_SOURCE_DIR}/core/pieces/include
)

add_google_test(queen_test
    SOURCES ${CMAKE_SOURCE_DIR}/core/pieces/src/queen.cpp ${CMAKE_SOURCE_DIR}/core/pieces/src/piece.cpp ${CMAKE_CURRENT_LIST_DIR}/src/queen_test.cpp
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
    add_test_unit_coverage(king_test_coverage ctest king_test)
    add_test_unit_coverage(knight_test_coverage ctest knight_test)
    add_test_unit_coverage(pawn_test_coverage ctest pawn_test)
    add_test_unit_coverage(queen_test_coverage ctest queen_test)
    add_test_unit_coverage(rook_test_coverage ctest rook_test)
endif (UNIX)
