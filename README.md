# Mathematics Engine - Calculus Toolkit

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![SDL2](https://img.shields.io/badge/SDL2-2.0+-orange.svg)

A Windows desktop application for **symbolic calculus** with **step-by-step solutions** and **interactive features**.

> 🎓 Comprehensive calculus toolkit supporting single and multivariate calculus operations with symbolic computation and real-time visualization.

## 🎯 Features

### **Seven Calculus Operations**
1. **Differentiation** (d/dx) - Find derivatives with step-by-step solutions
2. **Indefinite Integration** (∫ f(x) dx) - Find antiderivatives + C
3. **Definite Integration** (∫[a,b] f(x) dx) - Calculate area under curve
4. **Limits** (lim f(x)) - Calculate limits with step-by-step evaluation
5. **Matrix Multiplication** - Multiply matrices with custom dimensions
6. **Partial Derivatives** (∂f/∂x, ∂f/∂y) - Compute partial derivatives for multivariate functions
7. **Double Integration** (∫∫ f(x,y) dy dx) - Calculate double integrals over rectangular regions

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

#### Option A: Using GCC Directly (Recommended)

```bash
cd Mathh
mkdir build
cd build

g++ -std=c++17     -I/mingw64/include -I../src     ../src/main.cpp     ../src/engine/parser.cpp     ../src/engine/differentiator.cpp     ../src/engine/integrator.cpp     ../src/engine/simplifier.cpp     ../src/engine/limit_calculator.cpp     ../src/engine/matrix_operations.cpp     ../src/engine/latex_exporter.cpp     ../src/engine/partial_derivative.cpp     ../src/engine/multivariate_integrator.cpp     ../src/ui/renderer.cpp     ../src/ui/text_renderer.cpp     ../src/ui/plotter.cpp     -L/mingw64/lib     -o MathEngineUTF8.exe     -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf     -lfreetype -lharfbuzz -lgraphite2 -ldwrite -lole32     -lbz2 -lz -lglew32 -lopengl32     -mconsole -O2 -Wall
```

#### Option B: Using CMake

```bash
cd Mathh
mkdir build
cd build

# Generate build files
cmake -G "MinGW Makefiles" ..

# Compile
cmake --build .
```

### 4. Run the Application

```bash
./math_engine.exe
```

## 🐧 Linux Build Instructions

MathEngineUTF8 now supports native Linux builds 🎉  
Follow the steps below to compile and run it easily.

---

### 🧩 Prerequisites

Make sure you have all required development tools and libraries:

```bash
sudo apt update
sudo apt install build-essential pkg-config libsdl2-dev libsdl2-ttf-dev libglew-dev libgl1-mesa-dev
```

## ⚙️ Building the Project

### Clone the repository and switch into it:

```
git clone https://github.com/YOUR_USERNAME/Mathh.git
cd Mathh

```

 Run the included Linux build script:
 ```
 ./build/build_linux.sh
```

This script will:

1. Compile all source files using g++ -std=c++17

2. Use pkg-config to link against SDL2, SDL2_ttf, GLEW, and OpenGL

3. Output the executable MathEngineUTF8 inside the build/ directory


## Running the Program

```
./build/MathEngineUTF8

```
you should see output like :

```
+===================================+
|  Mathematics Engine - Calculus    |
+===================================+

```

### 💡 Notes

- The Linux build script is located at: `build/build_linux.sh`  
- You can edit it if you use a different compiler or custom flags.  
- The script won’t affect Windows or other build systems.



## 🎮 Controls

### Main Menu
| Key | Action |
|-----|--------|
| **1-7** | Select operation (1=Diff, 2=Indefinite, 3=Definite, 4=Limits, 5=Matrix, 6=Partial, 7=Double) |
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

### Definite Integration
| Key | Action |
|-----|--------|
| **B** | Change bounds |
| **TAB** | Switch between bounds |

### Limits
| Key | Action |
|-----|--------|
| **L** or **P** | Open limit configuration |
| **T** | Quick toggle limit type (finite/+∞/-∞) |
| **TAB** | Switch fields in config |

### Matrix Multiplication
| Key | Action |
|-----|--------|
| **TAB** | Switch between dimension fields |
| **ENTER** | Confirm and enter matrix values |

### Partial Derivatives
| Key | Action |
|-----|--------|
| **ENTER** | Type custom equation f(x,y) |
| **SPACE** | Next example |
| **X** | Export to LaTeX |

### Double Integration
| Key | Action |
|-----|--------|
| **B** | Set integration bounds (x and y) |
| **TAB** | Switch between bound fields |
| **SPACE** | Next example |

### How to Use

1. **Launch**: See main menu with 7 operations
2. **Select Mode**: Press 1-7 or use UP/DOWN arrows + ENTER
3. **Custom Input**: Press ENTER and type equation
4. **View Results**: See step-by-step solution
5. **Advanced Features**: Press B for bounds, L/P for limit config, X for LaTeX export
6. **Return**: Press ESC to go back to menu

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

**Partial Derivatives:**
```
x^2 + y^2           → ∂f/∂x = 2*x, ∂f/∂y = 2*y
x*y                 → ∂f/∂x = y, ∂f/∂y = x
sin(x)*cos(y)       → ∂f/∂x = cos(x)*cos(y), ∂f/∂y = -sin(x)*sin(y)
```

**Double Integration:**
```
x*y over [0,1]×[0,1]      → Result = 0.25
x^2 + y^2 over [0,2]×[0,2] → Numerical result
```

## 🏗️ Project Structure

```
MathEngine/
├── CMakeLists.txt           # Build configuration
├── README.md                # This file
├── src/
│   ├── main.cpp            # Application entry point
│   ├── engine/
│   │   ├── ast.h/.cpp                    # Abstract Syntax Tree definitions
│   │   ├── parser.h/.cpp                 # Expression parser
│   │   ├── differentiator.h/.cpp         # Symbolic differentiation
│   │   ├── integrator.h/.cpp             # Symbolic integration
│   │   ├── simplifier.h/.cpp             # Expression simplification
│   │   ├── limit_calculator.h/.cpp       # Limit evaluation
│   │   ├── matrix_operations.h/.cpp      # Matrix operations
│   │   ├── latex_exporter.h/.cpp         # LaTeX export
│   │   ├── partial_derivative.h/.cpp     # Partial derivatives
│   │   └── multivariate_integrator.h/.cpp # Double integration
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

- [x] Multiple variable support (partial derivatives, double integration)
- [x] Limits with L'Hôpital's rule
- [x] Matrix operations
- [x] LaTeX export
- [ ] Triple integration
- [ ] Gradient and divergence
- [ ] Taylor series expansion
- [ ] Visual AST tree representation
- [ ] ImGui-based UI for expression input
- [ ] Syntax highlighting in input field
- [ ] Symbol palette (∫, π, ∑, etc.)
- [ ] Equation solver

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Quick Start for Contributors
1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature`
3. Make your changes and test thoroughly
4. Commit with clear messages: `git commit -m "feat: add new feature"`
5. Push and create a pull request

### Areas We Need Help
- 🐛 Bug fixes and testing
- 📚 Documentation improvements
- ✨ New mathematical operations
- 🎨 UI/UX enhancements
- ⚡ Performance optimizations

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

**TL;DR**: Feel free to use, modify, and distribute this software. Attribution is appreciated but not required.

## 👥 Authors & Contributors

**Created by:** Shubh ([@positronx_](https://x.com/positronx_))

[![Twitter Follow](https://img.shields.io/twitter/follow/positronx_?style=social&logo=x)](https://x.com/positronx_)

**Contributors:** See [contributors list](https://github.com/shubhu121/Mathh/contributors)

Built with ❤️ using SDL2, OpenGL, and C++17 for Windows 10/11 (MSYS2 MinGW64)

## ☕ Support My Work

If you find this project helpful, consider supporting its development!

[![Buy Me A Coffee](https://img.shields.io/badge/Buy%20Me%20A%20Coffee-Support-yellow?style=for-the-badge&logo=buy-me-a-coffee)](https://buymeacoffee.com/positronx_)

Your support helps me:
- 🚀 Build more open-source tools
- 📚 Create educational content
- 🔧 Maintain and improve existing projects
- ☕ Stay caffeinated while coding!

[**Buy me a coffee ☕**](https://buymeacoffee.com/positronx_)

---

⭐ **Star this repository** if you find it helpful!  
🐛 **Report issues** to help us improve  
🔧 **Contribute** to make it even better!
