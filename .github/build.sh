#!/usr/bin/env bash
# @file      build.sh
# @brief     Build the application
# @copyright Copyright (C) Theodore Bardy. All rights reserved.
#            Developed by Theodore Bardy.
#            Reproduction, copy, modification in whole or part is prohibited
#            without the written permission of the copyright owner.

# Exit if a command returns a non-zero status
set -e

# Create build directory
mkdir -p build-ci

echo "######################################"
echo "--- Build Chess-Monitor repository ---"
echo "######################################"

# Move to build directory
cd build-ci

# Build applications | Following the "SC2044 – ShellCheck", prefer use while read
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
make clean
make -j"$(nproc)"

# Copy firmware
mkdir -p ../binaries
if [ -f chess_monitor.elf ]; then
    cp chess_monitor.elf ../binaries
fi

# Go back to top directory
cd ..
