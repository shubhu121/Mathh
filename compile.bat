@echo off
REM Manual compilation for Windows CMD/PowerShell
REM Run this from MSYS2 MinGW64 terminal or add MinGW64 to PATH

cd /d "%~dp0\build"

echo Compiling MathEngine...

g++ -std=c++17 ^
    -I/mingw64/include ^
    -I/mingw64/include/SDL2 ^
    -I../src ^
    ../src/main.cpp ^
    ../src/engine/parser.cpp ^
    ../src/engine/differentiator.cpp ^
    ../src/engine/simplifier.cpp ^
    ../src/engine/integrator.cpp ^
    ../src/engine/limit_calculator.cpp ^
    ../src/engine/matrix_operations.cpp ^
    ../src/engine/latex_exporter.cpp ^
    ../src/ui/renderer.cpp ^
    ../src/ui/text_renderer.cpp ^
    ../src/ui/plotter.cpp ^
    -L/mingw64/lib ^
    -lmingw32 ^
    -lSDL2main ^
    -lSDL2 ^
    -lSDL2_ttf ^
    -lglew32 ^
    -lopengl32 ^
    -lfreetype ^
    -lharfbuzz ^
    -lgraphite2 ^
    -lbz2 ^
    -lz ^
    -lpng ^
    -lrpcrt4 ^
    -mwindows ^
    -o MathEngineUTF8.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo Run: MathEngineUTF8.exe
) else (
    echo.
    echo Build failed!
    exit /b 1
)
