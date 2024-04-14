#!/usr/bin/env bash
# @file  build.sh
# @brief Build the application

# Exit if a command returns a non-zero status
set -e

# Create build directory
mkdir -p build-ci

echo "###################################"
echo "--- Build Chess-core repository ---"
echo "###################################"

# Move to build directory
cd build-ci

# Build applications | Following the "SC2044 – ShellCheck", prefer use while read
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
make clean
make -j"$(nproc)"

# Copy firmware
mkdir -p ../binaries
if [ -f chess_core.elf ]; then
    cp chess_core.elf ../binaries
fi

# Go back to top directory
cd ..
