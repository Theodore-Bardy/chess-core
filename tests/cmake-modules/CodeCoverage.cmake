# @file  CodeCoverage.cmake
# @brief Code coverage CMake module file for the unit tests

# 0. (Mac only) If you use Xcode 5.1 make sure to patch geninfo as described here:
#      http://stackoverflow.com/a/22404544/80480
# 1. Copy this file into your cmake modules path.
# 2. Add the following line to your CMakeLists.txt:
#      INCLUDE(CodeCoverage)
# 3. Set compiler flags to turn off optimization and enable coverage: 
#    SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
#     SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
# 4. Use the function add_test_unit_coverage to create a custom make target
#    which runs your test executable and produces a lcov code coverage report:
#    Example:
#     add_test_unit_coverage(
#                my_coverage_target  # Name for custom target.
#                test_driver         # Name of the test driver executable that runs the tests.
#                                    # NOTE! This should always have a ZERO as exit code
#                                    # otherwise the coverage generation will not complete.
#                coverage            # Name of output directory.
#                )
# 5. Build a Debug build:
#     make my_coverage_target

# Check prereqs
find_program( GCOV_PATH gcov )
find_program( LCOV_PATH lcov )
find_program( GENHTML_PATH genhtml )
find_program( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/tests)

if(NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif() # NOT GCOV_PATH
if(NOT LCOV_PATH)
    message(FATAL_ERROR "lcov not found! Aborting...")
endif() # NOT LCOV_PATH
if(NOT GENHTML_PATH)
    message(FATAL_ERROR "genhtml not found! Aborting...")
endif() # NOT GENHTML_PATH

# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests.
#                       MUST return ZERO always, even on errors.
#                       If not, no coverage report will be created!
# Param _outputname     lcov output is generated as _outputname.info
#                       HTML report is generated in _outputname/index.html
function(add_test_unit_coverage _targetname _testrunner _outputname)

    message("CodeCoverage: add target ${_targetname} ${_outputname}")

    # Setup target
    add_custom_target(${_targetname}
        
        # Cleanup lcov
        ${LCOV_PATH} --directory . --zerocounters
        
        # Run tests
        COMMAND ${_testrunner} ${ARGV3}
        
        # Capturing lcov counters and generating report
        COMMAND ${CMAKE_COMMAND} -E remove ${_outputname}.lcov
        COMMAND ${LCOV_PATH} --directory . --capture --output-file ${_outputname}.lcov
        COMMAND ${GENHTML_PATH} -o ${_outputname} ${_outputname}.lcov
        
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
    )
    
    # Show info where to find the report
    ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
        COMMAND ;
        COMMENT "Open ./${_outputname}/index.html in your browser to view the coverage report."
    )

endfunction()
