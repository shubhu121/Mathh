# Mathematics Engine - Calculus Toolkit (UTF-8)

A Windows desktop application for **symbolic calculus** with **step-by-step solutions** and **interactive features**.

## 🎯 Features

### **Three Calculus Operations**
1. **Differentiation** (d/dx) - Find derivatives with step-by-step solutions
2. **Indefinite Integration** (∫ f(x) dx) - Find antiderivatives + C
3. **Definite Integration** (∫[a,b] f(x) dx) - Calculate area under curve

### **Core Capabilities**
- **Custom Equation Input**: Type your own equations interactively
- **Step-by-Step Solutions**: View detailed calculations with mathematical notation
- **UTF-8 Support**: Full Unicode support for Greek letters (∂, π, α) and math symbols (⋅, √, ∫)
- **Interactive Menu**: Choose operation at startup
- **Multiple Examples**: Pre-loaded expressions for each mode
- **OpenGL Plotting**: Real-time graphs for all modes:
  - Differentiation: f(x) and f'(x) curves
  - Indefinite: f(x) and F(x) antiderivative curves
  - Definite: f(x) with shaded area between bounds
- **Bounds Input**: Set custom integration bounds for definite integrals

## 🛠️ Technologies

- **C++17**
- **SDL2** + **SDL2_ttf** for windowing and UTF-8 text rendering
- **OpenGL** for plotting and graphics
- **GLEW** for OpenGL extension loading
- **CMake** for cross-platform building

## 📦 Windows/MSYS2 Setup

### 1. Install MSYS2 MinGW64

Download and install MSYS2 from https://www.msys2.org/

### 2. Install Dependencies

Open MSYS2 MinGW64 terminal and run:

```bash
# Update package database
pacman -Syu

# Install compiler and build tools
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make

# Install SDL2 and graphics libraries
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
pacman -S mingw-w64-x86_64-glew mingw-w64-x86_64-glm
```

### 3. Build the Project

```bash
cd MathEngine
mkdir build
cd build

# Generate build files
cmake -G "MinGW Makefiles" ..

# Compile
cmake --build .
```

### 4. Run the Application

```bash
./MathEngineUTF8.exe
```

## 🎮 Controls

### Main Menu
| Key | Action |
|-----|--------|
| **1, 2, 3** | Select operation (Diff, Indefinite, Definite) |
| **UP/DOWN** | Navigate menu |
| **ENTER** | Confirm selection |
| **ESC** | Quit |

### Calculus Modes (All)
| Key | Action |
|-----|--------|
| **ENTER** | Type custom equation |
| **SPACE** | Next example |
| **UP/DOWN** | Scroll steps |
| **ESC** | Return to menu |

### Definite Integration (Additional)
| Key | Action |
|-----|--------|
| **B** | Change bounds |
| **TAB** | Switch between bounds |

### How to Use

1. **Launch**: See main menu with 3 operations
2. **Select Mode**: Press 1 (Diff), 2 (Indefinite), or 3 (Definite)
3. **Custom Input**: Press ENTER and type equation
4. **View Results**: See step-by-step solution
5. **Return**: Press ESC to go back to menu

## 📚 Supported Functions

### Basic Operations
- Addition: `x + 2`
- Subtraction: `x - 3`
- Multiplication: `x * 5` or `5x`
- Division: `x / 2`
- Exponentiation: `x^3`

### Trigonometric Functions
- `sin(x)`, `cos(x)`, `tan(x)`

### Other Functions
- `exp(x)` - Exponential
- `ln(x)` - Natural logarithm
- `sqrt(x)` - Square root

### Example Expressions

**Differentiation:**
```
sin(x^2)            → Chain rule
x^3 + 2*x           → Power rule
(x^3 + 2*x)*cos(x)  → Product rule
```

**Integration:**
```
x^2                 → Power rule (first/second degree)
sin(x)              → Trigonometric
exp(x)              → Exponential
```

**Definite Integration:**
```
x^2 from [0, 3]     → Area = 9.0
sin(x) from [0, π]  → Area = 2.0
```

## 🏗️ Project Structure

```
MathEngine/
├── CMakeLists.txt           # Build configuration
├── README.md                # This file
├── src/
│   ├── main.cpp            # Application entry point
│   ├── engine/
│   │   ├── ast.h           # Abstract Syntax Tree definitions
│   │   ├── parser.h/.cpp   # Expression parser
│   │   ├── differentiator.h/.cpp  # Symbolic differentiation
│   │   ├── integrator.h/.cpp      # Symbolic integration
│   │   └── simplifier.h/.cpp      # Expression simplification
│   └── ui/
│       ├── renderer.h/.cpp        # SDL2/OpenGL renderer
│       ├── text_renderer.h/.cpp   # UTF-8 text rendering
│       └── plotter.h/.cpp         # Function plotting
└── assets/
    └── fonts/
        └── NotoSans-Regular.ttf   # UTF-8 compatible font
```

## 🧮 How It Works

### 1. **Parsing**
- Input expression is tokenized and converted to Abstract Syntax Tree (AST)
- Supports operator precedence and parentheses
- Handles implicit multiplication (e.g., `2x` → `2 * x`)

### 2. **Differentiation**
- Applies calculus rules:
  - **Power Rule**: d/dx(xⁿ) = n·xⁿ⁻¹
  - **Chain Rule**: d/dx(f(g(x))) = f'(g(x))·g'(x)
  - **Product Rule**: d/dx(f·g) = f'·g + f·g'
  - **Quotient Rule**: d/dx(f/g) = (f'·g - f·g')/g²
- Records each step with description and expression

### 3. **Integration**
- Applies integration rules:
  - **Power Rule**: ∫ xⁿ dx = xⁿ⁺¹/(n+1) + C
  - **Constant Multiple**: ∫ c·f(x) dx = c·∫ f(x) dx
  - **Sum Rule**: ∫ (f + g) dx = ∫ f dx + ∫ g dx
  - **Trigonometric**: ∫ sin(x) dx = -cos(x) + C
- For definite: applies Fundamental Theorem (F(b) - F(a))
- Records each step with description

### 4. **Simplification**
- Eliminates redundant operations:
  - `x + 0` → `x`
  - `x * 1` → `x`
  - `x * 0` → `0`
  - `x^1` → `x`
- Constant folding: evaluates numeric expressions

### 5. **Rendering**
- **Text**: SDL2_ttf renders UTF-8 strings to OpenGL textures
- **Plotting**: OpenGL draws function graphs by sampling x values
- **UI**: Orthographic projection for 2D interface
- **Menu System**: Interactive mode selection

## 🔧 Customization

### Change Font Size
Edit `src/main.cpp`:
```cpp
textRenderer.init("assets/fonts/NotoSans-Regular.ttf", 24); // Change 24 to desired size
```

### Add New Functions
1. Add function to `UnaryFunc` enum in `src/engine/ast.h`
2. Implement derivative in `src/engine/differentiator.cpp`
3. Add parser support in `src/engine/parser.cpp`

### Adjust Plot Range
Edit `src/main.cpp`:
```cpp
plotter.setViewport(-5, 5, -5, 5); // xMin, xMax, yMin, yMax
```

## 🐛 Troubleshooting

### Font Not Found
Ensure `NotoSans-Regular.ttf` is in `build/assets/fonts/`

### Missing DLLs on Windows
Copy these from MinGW64 bin to build directory:
- SDL2.dll
- SDL2_ttf.dll
- libfreetype-6.dll
- glew32.dll
- libgcc_s_seh-1.dll
- libstdc++-6.dll
- libwinpthread-1.dll

### OpenGL Errors
Update graphics drivers to support OpenGL 2.1+

## 🚀 Future Enhancements

- [ ] Export steps as PDF with LaTeX formatting
- [ ] ImGui-based UI for expression input
- [ ] Integration and Taylor series expansion
- [ ] Visual AST tree representation
- [ ] Syntax highlighting in input field
- [ ] Symbol palette (∫, π, ∑, etc.)
- [ ] Multiple variable support
- [ ] Equation solver

## 📝 License

MIT License - Feel free to use and modify!

## 👨‍💻 Author

Built with SDL2, OpenGL, and C++17 for Windows 10/11 (MSYS2 MinGW64)
