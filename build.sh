#!/bin/bash

set -e

SRC_DIR="."
BUILD_DIR="./lin-build"
BUILD_WIN_DIR="./win-build"
OUT_LINUX="$BUILD_DIR/game"
OUT_WINDOWS="$BUILD_WIN_DIR/game.exe"
RAYLIB_WIN_DIR="../raylib-5.5_win64_mingw-w64"

mkdir -p "$BUILD_DIR" "$BUILD_WIN_DIR"

g++ "$SRC_DIR/main.cpp" -lraylib -ldl -lpthread -lm -lGL -o "$OUT_LINUX"

x86_64-w64-mingw32-g++ "$SRC_DIR/main.cpp" \
-I"$RAYLIB_WIN_DIR/include" \
-L"$RAYLIB_WIN_DIR/lib" \
-lraylib -lopengl32 -lgdi32 -lwinmm \
-static-libgcc -static-libstdc++ \
-o "$OUT_WINDOWS"

./win-build/game.exe
