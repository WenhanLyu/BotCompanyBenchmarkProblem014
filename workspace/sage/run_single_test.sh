#!/bin/bash
# Helper script to run a single test with proper library path and stdin redirection
export DYLD_LIBRARY_PATH=/opt/homebrew/opt/antlr4-cpp-runtime/lib:$DYLD_LIBRARY_PATH
exec ./code < "$1"
