# Integration Guide

## Overview
The Mathematics Engine now supports **three calculus operations**:
1. **Differentiation** - Find derivatives
2. **Indefinite Integration** - Find antiderivatives (+ C)
3. **Definite Integration** - Calculate area under curve with bounds

## Getting Started

### Main Menu
When you launch the application, you'll see:

```
╔═══════════════════════════════════════════════╗
║     MATHEMATICS ENGINE - Calculus Toolkit    ║
╚═══════════════════════════════════════════════╝

Select Operation:
  1. Differentiation (d/dx)
  2. Indefinite Integration (∫ f(x) dx)
  3. Definite Integration (∫[a,b] f(x) dx)

Controls:
  • Press 1, 2, or 3 to select
  • UP/DOWN arrows to navigate
  • ENTER to confirm
  • ESC to quit
```

## 1. Differentiation Mode

### How to Use
1. From menu, press **1** or select and press **ENTER**
2. See preset examples or press **ENTER** to type custom equation
3. View step-by-step derivative calculation
4. See graph of f(x) (blue) and f'(x) (red)

### Controls
- **ENTER** - Type custom equation
- **SPACE** - Next preset example
- **UP/DOWN** - Scroll through steps
- **ESC** - Return to menu

### Example Session
```
Input: f(x) = x^3 + 2*x

Step 1: Sum Rule: ∂/∂x(f + g) = f' + g'
Step 2: Power Rule: ∂/∂x(x^3) = 3·x^2
Step 3: Product Rule: ∂/∂x(2·x) = 2

Result: f'(x) = 3·x^2 + 2
```

### Supported Operations
- Power rule: x^n → n·x^(n-1)
- Chain rule: sin(x^2) → cos(x^2)·2x
- Product rule: f·g → f'·g + f·g'
- Quotient rule: f/g → (f'·g - f·g')/g²

---

## 2. Indefinite Integration Mode

### How to Use
1. From menu, press **2** or select and press **ENTER**
2. See preset examples or press **ENTER** to type custom equation
3. View step-by-step integration
4. Result shows antiderivative **+ C** (constant of integration)

### Controls
- **ENTER** - Type custom equation
- **SPACE** - Next preset example
- **UP/DOWN** - Scroll through steps
- **ESC** - Return to menu

### Example Session: First Degree Polynomial
```
Input: f(x) = x

Step 1: Initial expression
  ∫ x dx

Step 2: Power Rule: ∫ x dx = x²/2
  ∫ x dx = x²/2

Result: ∫ f(x) dx = x²/2 + C
```

### Example Session: Second Degree Polynomial
```
Input: f(x) = x^2 + 3*x + 1

Step 1: Sum Rule: ∫ (f + g) dx = ∫ f dx + ∫ g dx

Step 2: Power Rule: ∫ x^2 dx = x^3/3

Step 3: Constant Multiple Rule: ∫ 3·x dx = 3·∫ x dx

Step 4: Power Rule: ∫ x dx = x^2/2

Step 5: Constant Rule: ∫ 1 dx = x

Result: ∫ f(x) dx = x^3/3 + 3·x^2/2 + x + C
```

### Integration Rules Supported

#### Basic Rules
| Function | Integral |
|----------|----------|
| c (constant) | c·x + C |
| x | x²/2 + C |
| x^n | x^(n+1)/(n+1) + C |
| 1/x | ln\|x\| + C |

#### Trigonometric Functions
| Function | Integral |
|----------|----------|
| sin(x) | -cos(x) + C |
| cos(x) | sin(x) + C |

#### Exponential & Logarithmic
| Function | Integral |
|----------|----------|
| exp(x) | exp(x) + C |

#### Linearity Rules
- ∫ (f + g) dx = ∫ f dx + ∫ g dx
- ∫ (f - g) dx = ∫ f dx - ∫ g dx
- ∫ c·f(x) dx = c·∫ f(x) dx

---

## 3. Definite Integration Mode

### How to Use
1. From menu, press **3** or select and press **ENTER**
2. Default bounds are [0, 1]
3. Press **B** to change bounds
4. Press **ENTER** to type custom equation
5. View step-by-step calculation
6. Result shows **numerical value** (area under curve)

### Controls
- **ENTER** - Type custom equation
- **B** - Change integration bounds
- **SPACE** - Next preset example
- **UP/DOWN** - Scroll through steps
- **ESC** - Return to menu

### Setting Bounds
When you press **B**:
1. Enter lower bound (a)
2. Press **TAB** to switch to upper bound
3. Enter upper bound (b)
4. Press **ENTER** to compute

### Example Session: First Degree
```
Input: f(x) = x
Bounds: [0, 2]

Step 1: Initial expression
  ∫ x dx

Step 2: Power Rule: ∫ x dx = x²/2
  
Step 3: Fundamental Theorem: ∫[a,b] f(x) dx = F(b) - F(a)
  F(2) - F(0) = 2.0 - 0.0 = 2.0

Result: ∫[0,2] f(x) dx = 2.0
```

### Example Session: Second Degree
```
Input: f(x) = x^2
Bounds: [0, 3]

Step 1: Initial expression
  ∫ x^2 dx

Step 2: Power Rule: ∫ x^2 dx = x^3/3
  
Step 3: Fundamental Theorem: ∫[a,b] f(x) dx = F(b) - F(a)
  F(3) - F(0) = 9.0 - 0.0 = 9.0

Result: ∫[0,3] x^2 dx = 9.0
```

### Interpretation
- **Positive result**: Net area above x-axis
- **Negative result**: Net area below x-axis
- **Zero result**: Equal areas above/below, or function is zero

---

## Preset Examples

### Differentiation Presets
1. `sin(x^2)` - Trigonometric with chain rule
2. `x^3 + 2*x` - Polynomial
3. `(x^3 + 2*x)*cos(x)` - Product rule
4. `ln(x)*x^2` - Logarithm product
5. `exp(x)*sin(x)` - Exponential product

### Integration Presets
1. `x^2` - Simple power (second degree)
2. `x^3 + 2*x` - Polynomial
3. `sin(x)` - Trigonometric
4. `cos(x)` - Trigonometric
5. `exp(x)` - Exponential

---

## Custom Equations

### Syntax Rules
All modes use the same expression syntax:

**Variables**: `x` (lowercase only)

**Numbers**: `2`, `3.14`, `-5`

**Operators**: `+`, `-`, `*`, `/`, `^`

**Functions**: `sin`, `cos`, `tan`, `exp`, `ln`, `sqrt`

**Grouping**: `(...)` with nesting support

### Examples by Complexity

#### First Degree Polynomials
```
x
2*x
x + 5
3*x - 2
```

#### Second Degree Polynomials
```
x^2
x^2 + 2*x + 1
3*x^2 - 4*x + 7
(x + 1)^2
```

#### Higher Degrees
```
x^3
x^4 + 2*x^2
x^5 - 3*x^3 + x
```

#### Trigonometric
```
sin(x)
cos(x)
sin(x) + cos(x)
2*sin(x) - cos(x)
```

#### Mixed
```
x^2 + sin(x)
exp(x) + x^3
ln(x) + x^2
```

---

## Step-by-Step Explanations

### Differentiation Steps
Every differentiation shows:
1. **Rule name**: Power, Chain, Product, Quotient
2. **Formula**: Mathematical rule being applied
3. **Intermediate results**: Each transformation
4. **Simplified result**: Final answer

### Integration Steps
Every integration shows:
1. **Rule name**: Power, Sum, Constant Multiple
2. **Formula**: Integration rule being applied
3. **Intermediate results**: Each integration
4. **Final antiderivative**: Result + C (indefinite)
5. **Numerical evaluation**: F(b) - F(a) (definite)

---

## Tips & Best Practices

### For Indefinite Integration
1. **Always remember +C**: Constant of integration is shown
2. **Check by differentiating**: d/dx of integral should give original
3. **Simplify first**: Expand expressions before integrating
4. **Use linearity**: Break sums into separate integrals

### For Definite Integration
1. **Choose meaningful bounds**: Consider function behavior
2. **Check symmetry**: Odd functions on symmetric intervals = 0
3. **Split at discontinuities**: Avoid undefined points
4. **Interpret result**: Understand what area means

### Common Issues
❌ **Wrong**: `X^2` (uppercase)
✅ **Correct**: `x^2` (lowercase)

❌ **Wrong**: `sinx` (missing parentheses)
✅ **Correct**: `sin(x)`

❌ **Wrong**: `x/2*3` (ambiguous)
✅ **Correct**: `x/(2*3)` or `(x/2)*3`

---

## Keyboard Reference

### Menu Mode
| Key | Action |
|-----|--------|
| **1** | Select Differentiation |
| **2** | Select Indefinite Integration |
| **3** | Select Definite Integration |
| **UP/DOWN** | Navigate menu |
| **ENTER** | Confirm selection |
| **ESC** | Quit application |

### Calculus Modes (All)
| Key | Action |
|-----|--------|
| **ENTER** | Type custom equation |
| **SPACE** | Next preset example |
| **UP/DOWN** | Scroll steps |
| **ESC** | Return to menu |

### Definite Integration (Additional)
| Key | Action |
|-----|--------|
| **B** | Change integration bounds |
| **TAB** | Switch between bounds (while editing) |

### Input Mode (All)
| Key | Action |
|-----|--------|
| **Type** | Add characters |
| **BACKSPACE** | Delete character |
| **ENTER** | Compute |
| **ESC** | Cancel |

---

## Limitations & Future Features

### Current Limitations
- Only single variable `x` supported
- Complex chain rules may not fully simplify
- Product/quotient integration not implemented (use substitution manually)
- No partial fraction decomposition

### Planned Features
- [ ] More integration techniques
- [ ] Trigonometric substitution
- [ ] Integration by parts
- [ ] Numerical integration methods
- [ ] Multi-variable calculus
- [ ] Taylor series expansion
- [ ] Limit calculations

---

## Troubleshooting

### "Error: Unexpected character"
- Check for typos
- Ensure lowercase `x`
- Use proper function syntax: `sin(x)` not `sinx`

### Integration gives unexpected result
- Some functions may not simplify automatically
- Try breaking into simpler parts
- Check if function is supported

### Definite integral is NaN or infinite
- Check for discontinuities in bounds
- Ensure bounds are valid numbers
- Function may be undefined at bounds

### Bounds input not working
- Make sure you're in Definite Integration mode
- Press **B** to activate bounds editing
- Use **TAB** to switch between fields

---

## Mathematical Background

### Fundamental Theorem of Calculus
```
∫[a,b] f(x) dx = F(b) - F(a)
```
Where F(x) is the antiderivative of f(x).

### Relationship Between Operations
```
If F'(x) = f(x), then ∫ f(x) dx = F(x) + C
```

Differentiation and integration are **inverse operations**:
```
d/dx[∫ f(x) dx] = f(x)
∫[d/dx f(x)] dx = f(x) + C
```

---

## Examples Gallery

### Example 1: Polynomial Analysis
```
Function: f(x) = x^2 - 4*x + 3

Differentiation:
f'(x) = 2*x - 4

Indefinite Integration:
∫ f(x) dx = x^3/3 - 2*x^2 + 3*x + C

Definite Integration [0, 3]:
∫[0,3] f(x) dx = 0.0
```

### Example 2: Trigonometric
```
Function: f(x) = sin(x)

Differentiation:
f'(x) = cos(x)

Indefinite Integration:
∫ sin(x) dx = -cos(x) + C

Definite Integration [0, π]:
∫[0,π] sin(x) dx = 2.0
```

### Example 3: Exponential
```
Function: f(x) = exp(x)

Differentiation:
f'(x) = exp(x)

Indefinite Integration:
∫ exp(x) dx = exp(x) + C

Definite Integration [0, 1]:
∫[0,1] exp(x) dx ≈ 1.718
```

---

Enjoy exploring calculus! 📐∫
