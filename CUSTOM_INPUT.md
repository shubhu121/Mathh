# Custom Equation Input Guide

## Overview
The Mathematics Engine now supports **interactive custom equation input**, allowing you to differentiate any expression on-the-fly!

## How to Use

### Basic Usage
1. **Launch the application**: `./MathEngineUTF8.exe`
2. **Press ENTER**: Activates input mode
3. **Type your equation**: e.g., `x^5 + sin(x)*cos(x)`
4. **Press ENTER again**: Computes the derivative
5. **View results**: See step-by-step differentiation

### Input Mode Indicators
- **Yellow text**: "Type equation: [your text]_"
- **Gray hint**: "(Press ENTER to compute, ESC to cancel)"
- **Cursor**: Blinking underscore shows active input

### Canceling Input
- **Press ESC** while in input mode to cancel and return to normal view

## Supported Syntax

### Variables
- Use lowercase `x` as the variable

### Numbers
- Integers: `2`, `42`, `-5`
- Decimals: `3.14`, `0.5`, `-2.7`

### Basic Operations
| Operator | Syntax | Example |
|----------|--------|---------|
| Addition | `+` | `x + 5` |
| Subtraction | `-` | `x - 3` |
| Multiplication | `*` | `2*x` or `x*3` |
| Division | `/` | `x/2` |
| Exponentiation | `^` | `x^3` |

### Functions
| Function | Syntax | Example |
|----------|--------|---------|
| Sine | `sin(...)` | `sin(x)`, `sin(x^2)` |
| Cosine | `cos(...)` | `cos(x)`, `cos(2*x)` |
| Tangent | `tan(...)` | `tan(x)` |
| Exponential | `exp(...)` | `exp(x)`, `exp(-x)` |
| Natural Log | `ln(...)` | `ln(x)`, `ln(x+1)` |
| Square Root | `sqrt(...)` | `sqrt(x)`, `sqrt(x^2+1)` |

### Parentheses
- Use `()` for grouping: `(x+1)*(x-1)`
- Nested parentheses work: `sin(cos(x))`

### Implicit Multiplication
The parser supports implicit multiplication:
- `2x` → `2*x`
- `3(x+1)` → `3*(x+1)`
- `x(x+1)` → `x*(x+1)`

## Example Equations

### Beginner
```
x^2
x^3 + 5
2*x - 1
x^2 + x + 1
```

### Intermediate
```
sin(x)
cos(x^2)
x^2*sin(x)
ln(x)*x
exp(x) + x^2
```

### Advanced
```
(x^3 + 2*x)*cos(x)
sin(x)*cos(x)
exp(x)*sin(x)
ln(x^2 + 1)
sqrt(x^2 + 4)*x
```

### Complex
```
sin(exp(x))
ln(sin(x))
(x^2 + sin(x))/(x - 1)
exp(x^2)*cos(x)
sqrt(sin(x^2))
```

## Tips for Best Results

### ✅ Good Practices
- **Use parentheses**: Clear grouping prevents ambiguity
  - Good: `(x+1)*(x-1)`
  - Avoid: `x+1*x-1` (might parse unexpectedly)

- **Explicit operators**: Use `*` for clarity
  - Good: `2*x + 3*x^2`
  - Works but less clear: `2x + 3x^2`

- **Lowercase functions**: All functions are lowercase
  - Good: `sin(x)`, `cos(x)`, `ln(x)`
  - Bad: `Sin(x)`, `COS(x)`, `LN(x)`

### ❌ Common Mistakes

1. **Using uppercase X**
   - Bad: `X^2`
   - Good: `x^2`

2. **Missing parentheses on functions**
   - Bad: `sinx` or `sin x`
   - Good: `sin(x)`

3. **Division precedence**
   - `x/2*3` parses as `(x/2)*3`, not `x/(2*3)`
   - Use parentheses: `x/(2*3)`

4. **Multiple variables**
   - Bad: `x + y` (only `x` is supported)
   - Good: `x + 2` (use constants)

## Error Messages

If you enter an invalid expression, you'll see:

### "Unexpected character at position X"
- **Cause**: Invalid character in expression
- **Fix**: Check for typos, use only `x`, numbers, operators, functions

### "Expected closing parenthesis"
- **Cause**: Mismatched parentheses
- **Fix**: Ensure every `(` has a matching `)`

### "Unknown function: [name]"
- **Cause**: Unsupported function
- **Fix**: Use only: `sin`, `cos`, `tan`, `exp`, `ln`, `sqrt`

## Keyboard Reference

| Key | Action |
|-----|--------|
| **ENTER** (normal mode) | Activate input mode |
| **Type** (input mode) | Add characters to equation |
| **BACKSPACE** (input mode) | Delete last character |
| **ENTER** (input mode) | Compute derivative |
| **ESC** (input mode) | Cancel and return to normal mode |
| **SPACE** (normal mode) | Next example |
| **UP/DOWN** (normal mode) | Scroll steps |
| **ESC** (normal mode) | Exit application |

## Example Session

```
1. Press ENTER
   → Screen shows: "Type equation: _"

2. Type: x^4 + 2*x^2 - 5
   → Screen shows: "Type equation: x^4 + 2*x^2 - 5_"

3. Press ENTER
   → Computes derivative
   → Shows steps:
     Step 1: Sum Rule
     Step 2: Power Rule on x^4 → 4*x^3
     Step 3: Power Rule on 2*x^2 → 4*x
     Step 4: Constant Rule on -5 → 0
     Final: f'(x) = 4*x^3 + 4*x

4. View graph with f(x) in blue, f'(x) in red

5. Press ENTER again to try another equation
```

## Advanced Usage

### Nested Functions
```
sin(cos(x))           → -sin(x)*cos(cos(x))
exp(ln(x))            → 1 (simplifies to x, then to 1)
sqrt(x^2 + 1)         → x/sqrt(x^2 + 1)
```

### Product Rule Examples
```
x^2*sin(x)            → Shows product rule steps
ln(x)*x^3             → Shows product rule application
exp(x)*cos(x)         → Both terms differentiated
```

### Chain Rule Examples
```
sin(x^2)              → Shows chain rule: cos(x^2)*2*x
cos(3*x)              → Shows chain rule: -sin(3*x)*3
exp(x^2 + 1)          → Shows chain rule with exp
```

### Quotient Rule Examples
```
x/sin(x)              → Shows quotient rule
(x^2 + 1)/(x - 1)     → Full quotient rule derivation
sin(x)/x              → Quotient rule with trig
```

## Troubleshooting

### Input Not Appearing
- **Check**: Is input mode active? (Yellow "Type equation" text)
- **Fix**: Press ENTER to activate input mode

### Backspace Not Working
- **Check**: Is cursor position at end of text?
- **Note**: Only deletes from end of string

### Expression Not Computing
- **Check**: Did you press ENTER after typing?
- **Check**: Is expression valid? Look for error messages
- **Try**: Simplify expression, add parentheses

### Symbols Not Rendering
- **Check**: Font file exists in `assets/fonts/`
- **Fix**: Ensure NotoSans-Regular.ttf is present

## Future Enhancements

Planned features for custom input:
- [ ] Cursor movement (arrow keys)
- [ ] Copy/paste support
- [ ] Expression history (up/down arrows)
- [ ] Symbol palette (buttons for π, ∫, etc.)
- [ ] Syntax highlighting
- [ ] Auto-completion
- [ ] Multiple variables support

## Feedback

Found a bug or have a feature request? The parser currently supports common mathematical expressions. If you encounter parsing issues, try:
1. Adding explicit parentheses
2. Using `*` instead of implicit multiplication
3. Breaking complex expressions into simpler parts

Enjoy differentiating! 📐
