#!/bin/bash

set -e

SRC_DIR="."
BUILD_DIR="./build"
BUILD_WIN_DIR="./build-win"
OUT_LINUX="$BUILD_DIR/game"
OUT_WINDOWS="$BUILD_WIN_DIR/game.exe"
RAYLIB_WIN_DIR="../raylib-5.5_win64_mingw-w64"

# Create build directories
mkdir -p "$BUILD_DIR" "$BUILD_WIN_DIR"

# Compile for Linux
g++ "$SRC_DIR/main.cpp" -lraylib -ldl -lpthread -lm -lGL -o "$OUT_LINUX"

# Compile for Windows (cross-compile)
x86_64-w64-mingw32-g++ "$SRC_DIR/main.cpp" \
-I"$RAYLIB_WIN_DIR/include" \
-L"$RAYLIB_WIN_DIR/lib" \
-lraylib -lopengl32 -lgdi32 -lwinmm \
-static-libgcc -static-libstdc++ \
-o "$OUT_WINDOWS"
