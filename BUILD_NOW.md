# Build Instructions - Fix for Your Build Error

## ⚠️ Problem
You tried using manual `g++` command with a line break, which caused errors. **Use CMake instead.**

---

## ✅ Solution: Use MSYS2 MinGW64 (NOT PowerShell)

### Step 1: Open MSYS2 MinGW64
1. Press Windows key
2. Type: **MSYS2 MinGW64** (purple icon)
3. Click to open (NOT the blue MSYS2 icon)

### Step 2: Navigate to Project
```bash
cd /d/Mathh/build
```

### Step 3: Clean Previous Attempts
```bash
rm -f *.o MathEngineUTF8.exe
```

### Step 4: Build with CMake
```bash
# Generate build files
cmake -G "MinGW Makefiles" ..

# Compile
cmake --build . -j 4
```

### Step 5: Run
```bash
./MathEngineUTF8.exe
```

---

## 🚫 Don't Use Manual g++ Command

Your command had issues:
- ❌ Line break after `limit_calculator.cpp \` caused bash to interpret next line as command
- ❌ Missing implementations (they're in .cpp files that weren't compiled)
- ❌ WinMain error (need `-mwindows` flag or use SDL2main properly)

**CMake handles all this automatically!**

---

## 📋 Quick Copy-Paste Commands

Open MSYS2 MinGW64 and paste this:

```bash
cd /d/Mathh/build
rm -f *.o MathEngineUTF8.exe
cmake -G "MinGW Makefiles" ..
cmake --build . -j 4
./MathEngineUTF8.exe
```

---

## 🔍 If CMake Fails

### Error: "No CMakeLists.txt found"
```bash
# Make sure you're in the build directory
pwd
# Should show: /d/Mathh/build

# If not, navigate there
cd /d/Mathh/build
```

### Error: "CMake not found"
```bash
# Install CMake in MSYS2
pacman -S mingw-w64-x86_64-cmake
```

### Error: "Cannot find SDL2"
```bash
# Install dependencies
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-glew
```

---

## ✅ Expected Output

```
-- The C compiler identification is GNU 15.2.0
-- The CXX compiler identification is GNU 15.2.0
-- Found SDL2: TRUE
-- Configuring done
-- Generating done
-- Build files have been written to: /d/Mathh/build
[  9%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/main.cpp.obj
[ 18%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/engine/parser.cpp.obj
[ 27%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/engine/differentiator.cpp.obj
[ 36%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/engine/integrator.cpp.obj
[ 45%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/engine/simplifier.cpp.obj
[ 54%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/engine/limit_calculator.cpp.obj
[ 63%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/engine/matrix_operations.cpp.obj
[ 72%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/ui/renderer.cpp.obj
[ 81%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/ui/text_renderer.cpp.obj
[ 90%] Building CXX object CMakeFiles/MathEngineUTF8.dir/src/ui/plotter.cpp.obj
[100%] Linking CXX executable MathEngineUTF8.exe
[100%] Built target MathEngineUTF8
```

---

## 🎉 After Successful Build

Test the new features:
```bash
./MathEngineUTF8.exe
```

Then:
- Press **4** for Limits Calculator
- Press **5** for Matrix Multiplication

---

## 💡 Why Use CMake?

| CMake | Manual g++ |
|-------|------------|
| ✅ Handles all dependencies | ❌ Must specify everything |
| ✅ Cross-platform | ❌ Platform-specific |
| ✅ Handles linking automatically | ❌ Must specify all -l flags |
| ✅ Builds object files efficiently | ❌ Recompiles everything |
| ✅ Manages build directory | ❌ Clutters source directory |

CMake is the professional way to build C++ projects!
