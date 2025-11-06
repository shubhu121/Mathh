#!/bin/bash
# Build script for MathEngine with new features

echo "Building MathEngine with Limits and Matrix Multiplication..."

# Navigate to build directory
cd "$(dirname "$0")/build"

# Clean previous build
echo "Cleaning previous build..."
rm -f *.o MathEngineUTF8.exe

# Generate build files using CMake
echo "Generating build files with CMake..."
cmake -G "MinGW Makefiles" ..

# Build the project
echo "Compiling..."
cmake --build . -j 4

# Check if build was successful
if [ -f "MathEngineUTF8.exe" ]; then
    echo ""
    echo "✓ Build successful!"
    echo "Run with: ./MathEngineUTF8.exe"
    echo ""
else
    echo ""
    echo "✗ Build failed. Check errors above."
    echo ""
    exit 1
fi
