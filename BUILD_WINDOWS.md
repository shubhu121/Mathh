# Windows Build Instructions (MSYS2 MinGW64)

## Complete Setup Guide for Windows 10/11

### Step 1: Install MSYS2

1. Download MSYS2 installer from https://www.msys2.org/
2. Run `msys2-x86_64-YYYYMMDD.exe`
3. Install to default location: `C:\msys64`
4. Launch **MSYS2 MinGW64** terminal (NOT MSYS2 UCRT64 or MSYS2 MSYS)

### Step 2: Update System and Install Dependencies

```bash
# Update package database and core packages
pacman -Syu
# If prompted to close terminal, do so and reopen MSYS2 MinGW64

# Update remaining packages
pacman -Su

# Install C++ compiler and build tools
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make

# Install SDL2 and dependencies
pacman -S --needed mingw-w64-x86_64-SDL2 \
                   mingw-w64-x86_64-SDL2_ttf

# Install OpenGL libraries
pacman -S --needed mingw-w64-x86_64-glew \
                   mingw-w64-x86_64-glm
```

### Step 3: Verify Installation

```bash
# Check compiler version (should be gcc 13.x or newer)
gcc --version

# Check CMake (should be 3.27 or newer)
cmake --version

# Check installed packages
pacman -Qi mingw-w64-x86_64-SDL2
```

### Step 4: Navigate to Project Directory

```bash
# If you extracted MathEngine to Downloads folder:
cd /c/Users/YourUsername/Downloads/MathEngine

# Or use Windows path:
cd /c/path/to/MathEngine
```

### Step 5: Build the Project

```bash
# Create build directory
mkdir build
cd build

# Generate build files with MinGW Makefiles
cmake -G "MinGW Makefiles" ..

# Compile (use -j flag for parallel compilation)
cmake --build . -j 4

# Or use make directly
mingw32-make -j 4
```

### Step 6: Run the Application

```bash
# From build directory:
./MathEngineUTF8.exe

# Or double-click MathEngineUTF8.exe in Windows Explorer
```

## 🐛 Common Build Errors and Solutions

### Error: "SDL2 not found"

**Solution:**
```bash
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
```

### Error: "GLEW not found"

**Solution:**
```bash
pacman -S mingw-w64-x86_64-glew
```

### Error: "Cannot find -lSDL2"

**Cause:** Wrong terminal (using MSYS2 UCRT64 instead of MinGW64)

**Solution:** 
- Close current terminal
- Open **MSYS2 MinGW64** (purple icon)
- Rebuild

### Error: "Font not found" at runtime

**Solution:**
```bash
# Ensure font file exists
ls assets/fonts/NotoSans-Regular.ttf

# If missing, download it:
cd assets/fonts
wget https://github.com/notofonts/noto-fonts/raw/main/hinted/ttf/NotoSans/NotoSans-Regular.ttf
```

### Error: Missing DLLs when running .exe outside MSYS2

**Solution:** Copy DLLs to build directory:
```bash
cd build
cp /mingw64/bin/SDL2.dll .
cp /mingw64/bin/SDL2_ttf.dll .
cp /mingw64/bin/libfreetype-6.dll .
cp /mingw64/bin/glew32.dll .
cp /mingw64/bin/libgcc_s_seh-1.dll .
cp /mingw64/bin/libstdc++-6.dll .
cp /mingw64/bin/libwinpthread-1.dll .
cp /mingw64/bin/zlib1.dll .
cp /mingw64/bin/libbz2-1.dll .
cp /mingw64/bin/libharfbuzz-0.dll .
cp /mingw64/bin/libglib-2.0-0.dll .
cp /mingw64/bin/libgraphite2.dll .
cp /mingw64/bin/libintl-8.dll .
cp /mingw64/bin/libiconv-2.dll .
cp /mingw64/bin/libpcre2-8-0.dll .
cp /mingw64/bin/libpng16-16.dll .
cp /mingw64/bin/libbrotlidec.dll .
cp /mingw64/bin/libbrotlicommon.dll .
```

Or create a deployment script:

```bash
# Create deploy.sh in MathEngine root
cat > deploy.sh << 'EOF'
#!/bin/bash
cd build
ldd MathEngineUTF8.exe | grep mingw64 | awk '{print $3}' | xargs -I {} cp {} .
echo "DLLs copied successfully!"
EOF

chmod +x deploy.sh
./deploy.sh
```

## 📦 Creating Standalone Executable

To create a portable version that runs without MSYS2:

```bash
# From project root
cd build

# Copy all DLLs
ldd MathEngineUTF8.exe | grep mingw64 | awk '{print $3}' | xargs -I {} cp {} .

# Verify assets folder exists
ls -la assets/fonts/

# Create distribution folder
cd ..
mkdir -p MathEngine_Release
cp build/MathEngineUTF8.exe MathEngine_Release/
cp build/*.dll MathEngine_Release/
cp -r build/assets MathEngine_Release/
cp README.md MathEngine_Release/

# Compress to zip
7z a MathEngine_Windows.zip MathEngine_Release/
```

Now `MathEngine_Release` folder can be moved to any Windows PC!

## 🔧 Rebuild After Changes

```bash
cd build
cmake --build . -j 4
```

## 🧹 Clean Build

```bash
# Remove build directory
rm -rf build

# Rebuild from scratch
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build . -j 4
```

## 📌 Important Notes

1. **Always use MSYS2 MinGW64 terminal** (NOT MSYS2 MSYS or UCRT64)
2. **All source files must be UTF-8 encoded** (no BOM)
3. **Font file is required** at runtime in `assets/fonts/`
4. **For distribution**, include all DLLs or use static linking

## 🚀 Optional: Static Linking (Single EXE)

Edit `CMakeLists.txt` and add:

```cmake
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
    target_link_libraries(MathEngineUTF8
        ${SDL2_LIBRARIES}
        SDL2_ttf
        OpenGL::GL
        GLEW::GLEW
        -lmingw32
        -lSDL2main
        -Wl,-Bstatic -lpthread
    )
endif()
```

**Note:** Static linking increases .exe size but eliminates DLL dependencies.

## 🎯 Performance Tips

### Faster Compilation
```bash
# Use all CPU cores
cmake --build . -j $(nproc)
```

### Release Build (Optimized)
```bash
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j 4
```

### Debug Build (for development)
```bash
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j 4
```

## 📞 Getting Help

If build fails:

1. **Check terminal**: Must be MSYS2 MinGW64
2. **Verify packages**: `pacman -Qi mingw-w64-x86_64-SDL2`
3. **Clean build**: `rm -rf build && mkdir build`
4. **Check CMake output**: Look for "SDL2 found" messages
5. **Test simple program**: Compile SDL2 hello world first

## 🎉 Success Indicators

You should see:
```
-- The C compiler identification is GNU 13.x.x
-- The CXX compiler identification is GNU 13.x.x
-- Found SDL2: TRUE
-- Found OpenGL: opengl32
-- Build files have been written to: .../build
[100%] Built target MathEngineUTF8
```

Run test:
```bash
./MathEngineUTF8.exe
```

Should display window with UTF-8 math symbols and interactive plot!

## 🎮 Using the Application

Once running:
1. **Press ENTER** to type your own equation (e.g., `x^4 + sin(2*x)`)
2. **Press SPACE** to cycle through built-in examples
3. **Use UP/DOWN** to scroll through steps
4. **Press ESC** to exit (or cancel input mode)
