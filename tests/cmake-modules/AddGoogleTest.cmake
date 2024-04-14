# @file  AddGoogleTest.cmake
# @brief Add GoogleTest CMake module file

# AddGoogleTest
# -------------
#
# This file provides a function to add a google test
#
#
# ::
#
#   add_google_test(target_name
#                   SOURCES src1 src2 ... srcN
#                   [COMPILE_OPTIONS opt1 opt2 ... optN]
#                   [LINK_LIBRARIES lib1 lib2 ... libN]
#                   [INCLUDE_DIRECTORIES dir1 dir2 .. dirN]
#                  )
#
# ``target_name``:
#   Required, expects the name of the test which will be used to define a target
#
# ``SOURCES``:
#   Required, expects one or more source files names
#
# ``COMPILE_OPTIONS``:
#   Optional, expects one or more options to be passed to the compiler
#
# ``LINK_LIBRARIES``:
#   Optional, expects one or more libraries to be linked with the test
#   executable.
#
# ``INCLUDE_DIRECTORIES``:
#   Optional, expects one or more directories to be included as include directories for tests
#
# Example:
#
# .. code-block:: cmake
#
#   add_google_test(my_test
#                   SOURCES my_test.c other_source.c
#                   COMPILE_OPTIONS -g -Wall
#                   LINK_LIBRARIES mylib
#                   INCLUDE_DIRECTORIES some/path/with/headers_files
#                  )
#

# Enable testing for current directory and below
enable_testing()
include(GoogleTest)

function(ADD_GOOGLE_TEST _TARGET_NAME)

    set(one_value_arguments
    )

    set(multi_value_arguments
        SOURCES
        COMPILE_OPTIONS
        LINK_LIBRARIES
        INCLUDE_DIRECTORIES
    )

    cmake_parse_arguments(_add_google_test
        ""
        "${one_value_arguments}"
        "${multi_value_arguments}"
        ${ARGN}
    )

    if(NOT DEFINED _add_google_test_SOURCES)
        message(FATAL_ERROR "No sources provided for target ${_TARGET_NAME}")
    endif()

    add_executable(${_TARGET_NAME} ${_add_google_test_SOURCES})

    if (DEFINED _add_google_test_COMPILE_OPTIONS)
        target_compile_options(${_TARGET_NAME}
            PRIVATE ${_add_google_test_COMPILE_OPTIONS}
        )
    endif()

    if(DEFINED _add_google_test_LINK_LIBRARIES)
        target_link_libraries(${_TARGET_NAME}
            PRIVATE ${_add_google_test_LINK_LIBRARIES}
        )
    endif()

    if(DEFINED _add_google_test_INCLUDE_DIRECTORIES)
        target_include_directories(${_TARGET_NAME} PRIVATE ${_add_google_test_INCLUDE_DIRECTORIES})
    endif()


    gtest_discover_tests(${_TARGET_NAME})

endfunction(ADD_GOOGLE_TEST)
