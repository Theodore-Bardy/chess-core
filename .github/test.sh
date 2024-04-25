#!/usr/bin/env bash
# @file  test.sh
# @brief Build and run unit tests

# Exit if a command returns a non-zero status
set -e

# Create build directory
mkdir -p build-test
cd build-test

echo "##############################################"
echo "--- Build Chess-core repository unit tests ---"
echo "##############################################"

# Build unit tests
cmake .. -DCMAKE_BUILD_TYPE=Debug -DUNIT_TESTS=ON
cmake --build .

# Execute tests
ctest --stop-on-failure --output-on-failure --parallel "$(nproc)"

# Generate gcov reports
for gcda_file in `find . -name *.gcda`
do
    (cd `dirname ${gcda_file}` && gcov `basename ${gcda_file}`)
done

# Go back to top directory
cd ..
