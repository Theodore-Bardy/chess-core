#!/usr/bin/env bash
# @file  ci_entrypoint.sh
# @brief Entry point for the CI (must be the first script executed)

# Specify the directory is safe
sh -c "git config --global --add safe.directory $PWD"
