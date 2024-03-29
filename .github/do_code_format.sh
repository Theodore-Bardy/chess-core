#!/usr/bin/env bash
# @file  do_code_format.sh
# @brief Update all source files according to wanted code format

# Update source and header files
for source_file in `git ls-tree -r HEAD --name-only | grep -E '(.*\.c$|.*\.cpp$|.*\.h$|.*\.hpp$)'`
do
    clang-format -i ${source_file} || exit 1
done

exit 0
