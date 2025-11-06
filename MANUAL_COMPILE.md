# Manual g++ Compilation Command

## ✅ Working Command (No Line Breaks)

Open **MSYS2 MinGW64** terminal and navigate to build directory:
```bash
cd /d/Mathh/build
```

Then run this **single command** (copy the entire thing):

```bash
g++ -std=c++17 -I/mingw64/include -I/mingw64/include/SDL2 -I../src ../src/main.cpp ../src/engine/parser.cpp ../src/engine/differentiator.cpp ../src/engine/simplifier.cpp ../src/engine/integrator.cpp ../src/engine/limit_calculator.cpp ../src/engine/matrix_operations.cpp ../src/engine/latex_exporter.cpp ../src/ui/renderer.cpp ../src/ui/text_renderer.cpp ../src/ui/plotter.cpp -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lglew32 -lopengl32 -lfreetype -lharfbuzz -lgraphite2 -lbz2 -lz -lpng -lrpcrt4 -mwindows -o MathEngineUTF8.exe
```

---

## 📋 Or Use the Script

I created `compile_manual.sh` - just run:
```bash
cd /d/Mathh
chmod +x compile_manual.sh
./compile_manual.sh
```

---

## 🔍 What Fixed Your Previous Command

### Problems in Your Original Command:
1. ❌ **Line break issue** - You had a blank line after `limit_calculator.cpp \`
2. ❌ **Missing `-mwindows` flag** - Caused "WinMain" error
3. ❌ **Wrong library order** - SDL2main must come before SDL2
4. ❌ **Missing `-I/mingw64/include/SDL2`** - SDL2 headers need explicit path

### Fixed Version:
- ✅ All on one line (or proper backslashes with no blank lines)
- ✅ Added `-mwindows` flag for Windows GUI app
- ✅ Correct library link order: `-lmingw32 -lSDL2main -lSDL2`
- ✅ Added SDL2 include path: `-I/mingw64/include/SDL2`
- ✅ Added all source files including new ones

---

## 📖 Explanation of Flags

| Flag | Purpose |
|------|---------|
| `-std=c++17` | Use C++17 standard |
| `-I/mingw64/include` | Include system headers |
| `-I/mingw64/include/SDL2` | SDL2 headers location |
| `-I../src` | Your project headers |
| `-L/mingw64/lib` | Library search path |
| `-lmingw32` | MinGW runtime (MUST be first) |
| `-lSDL2main` | SDL2 main entry point (before SDL2) |
| `-lSDL2` | SDL2 library |
| `-lSDL2_ttf` | SDL2 TrueType font support |
| `-lglew32` | OpenGL Extension Wrangler |
| `-lopengl32` | OpenGL library |
| `-lfreetype` | Font rendering |
| `-lharfbuzz` | Text shaping (for fonts) |
| `-lgraphite2` | Font rendering support |
| `-lbz2` | Compression library |
| `-lz` | zlib compression |
| `-lpng` | PNG image support |
| `-lrpcrt4` | Windows RPC runtime |
| `-mwindows` | Build as Windows GUI app (fixes WinMain) |
| `-o MathEngineUTF8.exe` | Output executable name |

---

## ⚠️ Important Notes

### 1. Library Order Matters!
The order **must be**:
```
-lmingw32 -lSDL2main -lSDL2 [other libs...]
```

If you put SDL2main after SDL2, you get "WinMain" errors!

### 2. No Blank Lines in Multi-line Commands
If using backslashes, don't have blank lines:

**❌ Wrong:**
```bash
g++ file1.cpp \

file2.cpp \
```

**✅ Correct:**
```bash
g++ file1.cpp \
    file2.cpp \
    file3.cpp
```

### 3. Use MSYS2 MinGW64 Terminal
- Not PowerShell
- Not CMD
- Not regular MSYS2

---

## 🚀 Quick Test

After compilation:
```bash
./MathEngineUTF8.exe
```

Then:
- Press **4** → Limits Calculator
- Press **5** → Matrix Multiplication

---

## 💡 Why Your Original Command Failed

Your command was:
```bash
g++ ... \
    ../src/engine/limit_calculator.cpp \
 
    ../src/engine/matrix_operations.cpp \
```

See that **blank line** after `limit_calculator.cpp \`? 

The backslash only continues to the **very next line**. The blank line broke the continuation, so bash tried to run `../src/engine/matrix_operations.cpp` as a command!

That's why you saw:
```
../src/engine/matrix_operations.cpp: line 6: syntax error near unexpected token `int'
```

Bash was trying to execute your .cpp file as a shell script! 😅

---

## ✅ The Working Solution

Use the one-liner at the top, or use `compile_manual.sh` which has proper backslashes with **no blank lines**.
