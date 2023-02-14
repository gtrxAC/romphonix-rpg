#!/bin/bash
# ______________________________________________________________________________
#
#  Build options
#  This script is sourced by build.sh and setup.sh, no need to run it yourself
# ______________________________________________________________________________
#
# Executable name, extension is added depending on target platform.
NAME=romphonix

# Files to compile. You can add multiple files by separating by spaces.
SRC="src/*.c"

# Platform, one of Windows_NT, Linux, Web, Android. Defaults to your OS.
# This can be set from the command line: TARGET=Android ./build.sh
[[ -z "$TARGET" ]] && TARGET=$(uname)

# Compiler flags.
# This can be set from the command line: FLAGS="-Ofast" ./build.sh

# Using functions before they are declared is against best practices but it
# shouldn't break anything, and it will make our lives a lot easier
[[ -z "$FLAGS" ]] && FLAGS="-Wno-implicit-function-declaration"

# Compiler flags for release and debug mode
# To set debug mode, run: DEBUG=1 ./build.sh
RELEASE_FLAGS="-Os -flto -s"
DEBUG_FLAGS="-DDEBUG -O0 -g -Wall -Wextra -Wpedantic"