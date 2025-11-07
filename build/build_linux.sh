#!/bin/bash
set -e

echo "🧱 Building MathEngineUTF8 for Linux..."

# Go to the project root (one level up)
cd "$(dirname "$0")/.."

# Create build directory if it doesn’t exist
mkdir -p build
cd build

# Compile using your verified working command
g++ -std=c++17 \
    -I../src \
    $(pkg-config --cflags sdl2 glew gl) \
    ../src/main.cpp \
    ../src/engine/*.cpp \
    ../src/ui/*.cpp \
    -o MathEngineUTF8 \
    $(pkg-config --libs sdl2 SDL2_ttf glew gl) \
    -O2 -Wall

echo "✅ Build complete!"
echo "Run it with: ./build/MathEngineUTF8"
