# OpenGL Rendering Guide

## Overview
The Mathematics Engine features **real-time OpenGL plotting** for all three calculus operations, providing visual feedback alongside step-by-step solutions.

## 📊 Rendering Features by Mode

### 1. **Differentiation Mode**
**What you see:**
- **Blue curve**: Original function f(x)
- **Red curve**: Derivative f'(x)
- Grid and axes for reference

**Example:**
```
f(x) = x^2
f'(x) = 2x

Visual: Parabola (blue) with its tangent slope (red line)
```

**Use cases:**
- Verify derivative visually
- Compare slopes at different points
- Understand rate of change

---

### 2. **Indefinite Integration Mode** (NEW!)
**What you see:**
- **Blue curve**: Original function f(x)
- **Green curve**: Antiderivative F(x) = ∫f(x)dx
- Grid and axes for reference

**Example:**
```
f(x) = 2x
F(x) = x^2 + C

Visual: Linear function (blue) with its parabolic integral (green)
```

**Key insights:**
- F(x) is "one degree higher" than f(x)
- Slope of F(x) at any point = value of f(x)
- Visual proof of antiderivative relationship

---

### 3. **Definite Integration Mode** (NEW!)
**What you see:**
- **Blue curve**: Function f(x)
- **Shaded area**: Region between curve and x-axis from a to b
- **Vertical bounds**: Lines at x = a and x = b
- Grid and axes for reference

**Example:**
```
f(x) = x^2
Bounds: [0, 3]
Area = 9.0

Visual: Parabola with shaded region from x=0 to x=3
```

**Key insights:**
- Shaded area = numerical result
- Visual representation of "area under curve"
- Positive area above x-axis, negative below
- Bounds clearly marked with vertical lines

---

## 🎨 Visual Elements

### Color Coding
| Element | Color | Purpose |
|---------|-------|---------|
| Grid | Dark gray | Reference lines |
| Axes | Medium gray | X and Y axes (thicker) |
| f(x) (Diff) | Blue | Original function |
| f'(x) | Red | Derivative |
| f(x) (Indef) | Blue | Integrand |
| F(x) | Green | Antiderivative |
| f(x) (Def) | Dark blue | Function to integrate |
| Shaded area | Light blue (40% alpha) | Definite integral result |
| Bound lines | Blue | Vertical lines at a and b |

### Plot Features
- **600x300 pixel viewport** on right side of screen
- **Automatic scaling**: Viewport set to [-5, 5] x [-5, 5]
- **Smooth curves**: High-resolution sampling (1200 points)
- **Alpha blending**: Shaded areas with transparency
- **Anti-aliasing**: Smooth lines and fills

---

## 🔧 Technical Details

### Rendering Pipeline

1. **Setup Viewport**
   ```cpp
   glViewport(plotX, plotY, 600, 300);
   glMatrixMode(GL_PROJECTION);
   glOrtho(0, 600, 300, 0, -1, 1);
   ```

2. **Draw Grid & Axes**
   - Grid: 1-unit spacing
   - Axes: Highlighted at x=0, y=0

3. **Plot Functions**
   - Sample function at regular intervals
   - Convert to screen coordinates
   - Draw using GL_LINE_STRIP

4. **Fill Areas (Definite only)**
   - Use GL_TRIANGLE_STRIP
   - Enable alpha blending
   - Fill from x-axis to curve

5. **Restore Viewport**
   - Return to full window projection

### Coordinate Systems

**World Coordinates** (mathematical):
- x: [-5, 5]
- y: [-5, 5]

**Screen Coordinates** (pixels):
- x: [0, 600]
- y: [0, 300]

**Conversion:**
```
screenX = (x - xMin) / (xMax - xMin) * width
screenY = height - (y - yMin) / (yMax - yMin) * height
```

---

## 🎯 Use Cases

### Differentiation Visualization
**Verify derivative correctness:**
- Horizontal tangent → f'(x) = 0
- Steep slope → large |f'(x)|
- Increasing function → f'(x) > 0

**Example: f(x) = x²**
- At x=0: flat, f'(0) = 0 ✓
- At x=2: steep, f'(2) = 4 ✓

### Indefinite Integration Visualization
**Understand antiderivatives:**
- F(x) accumulates area under f(x)
- Steeper F(x) → larger f(x)
- Constant f(x) → linear F(x)

**Example: f(x) = 2x**
- Constant slope → parabola
- F(x) = x² grows quadratically ✓

### Definite Integration Visualization
**Interpret area under curve:**
- See exact region being integrated
- Positive area above x-axis
- Negative area below x-axis
- Bounds clearly marked

**Example: ∫[0,2] x² dx = 2.67**
- Shaded parabolic segment
- Visual confirmation of result ✓

---

## 🖼️ Graph Interpretation Guide

### Reading the Plots

**Differentiation:**
```
Blue crosses x-axis at x=2
  → f(2) = 0

Red crosses x-axis at x=1
  → f'(1) = 0 (critical point of f)

Red is positive when blue is increasing
  → Slope relationship confirmed
```

**Indefinite Integration:**
```
Blue is constant at y=2
  → f(x) = 2

Green is linear with slope 2
  → F(x) = 2x (+ C)
  → Relationship confirmed
```

**Definite Integration:**
```
Shaded area from x=0 to x=3
Curve: y = x²
Area ≈ 9.0

Visual check:
- Rectangle 3×9 = 27
- Shaded area ≈ 1/3 rectangle ✓
```

---

## ⚙️ Customization

### Adjust Viewport Range
In `main.cpp`:
```cpp
plotter.setViewport(-10, 10, -10, 10); // Zoom out
plotter.setViewport(-2, 2, -2, 2);     // Zoom in
```

### Change Colors
```cpp
// Differentiation
plotter.plotFunction(ast.get(), 1.0f, 0.0f, 0.0f); // Red f(x)
plotter.plotFunction(result.get(), 0.0f, 0.0f, 1.0f); // Blue f'(x)

// Indefinite
plotter.plotFunction(ast.get(), 1.0f, 0.5f, 0.0f); // Orange f(x)
plotter.plotFunction(result.get(), 1.0f, 0.0f, 1.0f); // Magenta F(x)

// Definite fill
plotter.plotFunctionFilled(ast.get(), a, b, 1.0f, 0.0f, 0.0f, 0.5f); // Red fill
```

### Adjust Line Thickness
```cpp
plotter.plotFunction(ast.get(), r, g, b, 3.0f); // Thicker line
plotter.plotFunction(result.get(), r, g, b, 1.0f); // Thinner line
```

### Change Fill Transparency
```cpp
// More opaque
plotter.plotFunctionFilled(ast.get(), a, b, r, g, bl, 0.7f);

// More transparent
plotter.plotFunctionFilled(ast.get(), a, b, r, g, bl, 0.2f);
```

---

## 🐛 Troubleshooting

### Graph Not Showing
**Check:**
1. Is parseSuccess = true?
2. Are there steps in the vector?
3. Is the function defined in viewport range?

**Fix:**
- Add error checking before plot calls
- Adjust viewport to match function domain

### Function Goes Off Screen
**Cause:** Function values exceed [-5, 5] range

**Fix:**
```cpp
plotter.setViewport(-10, 10, -20, 20); // Wider range
```

### Shaded Area Wrong
**Cause:** Bounds outside viewport or function undefined

**Fix:**
- Ensure a, b within [-5, 5]
- Check function has no discontinuities

### Performance Issues
**Cause:** Too many plot points

**Fix:** Reduce sampling in `plotter.cpp`:
```cpp
int numPoints = width / 2; // Half as many points
```

---

## 📐 Mathematical Accuracy

### Sampling Resolution
- **Differentiation/Indefinite**: 1200 points
- **Definite fill**: 200 points
- **Sufficient for smooth curves** on 600px width

### Clamping
Functions are clamped to viewport:
```cpp
if (y >= yMin && y <= yMax) {
    // Plot point
}
```

Prevents rendering artifacts from:
- Asymptotes
- Undefined points
- Infinity values

### Numerical Stability
- Uses `std::isfinite()` checks
- Clamps extreme values
- Skips undefined regions

---

## 🎨 Visual Examples

### Polynomial
```
f(x) = x³ - 3x

Diff mode:
- Blue: S-curve
- Red: Parabola (opens up)
- Red crosses axis at ±1 (critical points)

Indef mode:
- Blue: Cubic
- Green: Quartic
```

### Trigonometric
```
f(x) = sin(x)

Diff mode:
- Blue: Sine wave
- Red: Cosine wave (90° phase shift)

Def mode [0, π]:
- Blue: Sine arch
- Shaded: Area = 2.0
```

### Exponential
```
f(x) = exp(x)

Diff mode:
- Blue: Exponential curve
- Red: Same curve (exp' = exp)

Indef mode:
- Blue: Exponential
- Green: Same exponential (+ C)
```

---

## 🚀 Future Enhancements

Planned rendering features:
- [ ] 3D surface plots
- [ ] Parametric curves
- [ ] Polar coordinates
- [ ] Vector fields
- [ ] Tangent line overlay
- [ ] Normal line display
- [ ] Zoom/pan controls
- [ ] Export to PNG
- [ ] Animation mode
- [ ] Multiple function overlay

---

Enjoy the visual calculus experience! 📊🎨
