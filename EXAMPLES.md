# Example Expressions and Expected Output

## Basic Examples

### 1. Simple Power Function
**Input:** `x^2`

**Steps:**
```
Step 1: Power Rule: ∂/∂x(x^n) = n ⋅ x^(n-1)
  ∂/∂x(x^2) = 2 ⋅ x^1

Step 2: Simplification
  f'(x) = 2 ⋅ x
```

### 2. Polynomial
**Input:** `x^3 + 2*x`

**Steps:**
```
Step 1: Sum Rule: ∂/∂x(f + g) = f' + g'
Step 2: Power Rule on x^3: ∂/∂x(x^3) = 3 ⋅ x^2
Step 3: Product Rule on 2*x: ∂/∂x(2 ⋅ x) = 2
Final: f'(x) = 3 ⋅ x^2 + 2
```

### 3. Sine Function
**Input:** `sin(x^2)`

**Steps:**
```
Step 1: Chain Rule: ∂/∂x(sin(u)) = cos(u) ⋅ u'
  ∂/∂x(sin(x^2)) = cos(x^2) ⋅ ∂/∂x(x^2)

Step 2: Power Rule: ∂/∂x(x^2) = 2 ⋅ x

Final: f'(x) = 2 ⋅ x ⋅ cos(x^2)
```

## Advanced Examples

### 4. Product of Polynomial and Trig
**Input:** `(x^3 + 2*x)*cos(x)`

**Steps:**
```
Step 1: Product Rule: ∂/∂x(f ⋅ g) = f' ⋅ g + f ⋅ g'
Step 2: Differentiate x^3 + 2*x
Step 3: Differentiate cos(x) → -sin(x)
Final: f'(x) = (3 ⋅ x^2 + 2) ⋅ cos(x) + (x^3 + 2 ⋅ x) ⋅ (-sin(x))
```

### 5. Logarithmic Function
**Input:** `ln(x)*x^2`

**Steps:**
```
Step 1: Product Rule: ∂/∂x(ln(x) ⋅ x^2)
Step 2: ∂/∂x(ln(x)) = 1/x
Step 3: ∂/∂x(x^2) = 2 ⋅ x
Final: f'(x) = (1/x) ⋅ x^2 + ln(x) ⋅ 2 ⋅ x = x + 2 ⋅ x ⋅ ln(x)
```

### 6. Exponential with Trig
**Input:** `exp(x)*sin(x)`

**Steps:**
```
Step 1: Product Rule
Step 2: ∂/∂x(exp(x)) = exp(x)
Step 3: ∂/∂x(sin(x)) = cos(x)
Final: f'(x) = exp(x) ⋅ sin(x) + exp(x) ⋅ cos(x)
```

## Trigonometric Examples

### 7. Cosine Squared
**Input:** `cos(x)^2`

**Steps:**
```
Step 1: Power Rule with Chain Rule
  ∂/∂x(cos(x)^2) = 2 ⋅ cos(x) ⋅ ∂/∂x(cos(x))

Step 2: Derivative of cos(x)
  ∂/∂x(cos(x)) = -sin(x)

Final: f'(x) = -2 ⋅ cos(x) ⋅ sin(x)
```

### 8. Tangent Function
**Input:** `tan(x^2)`

**Steps:**
```
Step 1: Chain Rule: ∂/∂x(tan(u)) = sec²(u) ⋅ u'
Step 2: sec²(x^2) = 1/cos²(x^2)
Step 3: ∂/∂x(x^2) = 2 ⋅ x
Final: f'(x) = (2 ⋅ x) / cos²(x^2)
```

## Composite Functions

### 9. Nested Functions
**Input:** `sin(cos(x))`

**Steps:**
```
Step 1: Chain Rule (outer): ∂/∂x(sin(cos(x))) = cos(cos(x)) ⋅ ∂/∂x(cos(x))
Step 2: Chain Rule (inner): ∂/∂x(cos(x)) = -sin(x)
Final: f'(x) = -cos(cos(x)) ⋅ sin(x)
```

### 10. Exponential of Polynomial
**Input:** `exp(x^3 + 2*x)`

**Steps:**
```
Step 1: Chain Rule: ∂/∂x(exp(u)) = exp(u) ⋅ u'
Step 2: ∂/∂x(x^3 + 2*x) = 3 ⋅ x^2 + 2
Final: f'(x) = exp(x^3 + 2 ⋅ x) ⋅ (3 ⋅ x^2 + 2)
```

## Quotient Examples

### 11. Simple Quotient
**Input:** `x/sin(x)`

**Steps:**
```
Step 1: Quotient Rule: ∂/∂x(f/g) = (f' ⋅ g - f ⋅ g') / g²
Step 2: ∂/∂x(x) = 1
Step 3: ∂/∂x(sin(x)) = cos(x)
Final: f'(x) = (sin(x) - x ⋅ cos(x)) / sin²(x)
```

### 12. Polynomial Quotient
**Input:** `(x^2 + 1)/(x - 1)`

**Steps:**
```
Step 1: Quotient Rule
Step 2: Numerator derivative: 2 ⋅ x
Step 3: Denominator derivative: 1
Final: f'(x) = (2 ⋅ x ⋅ (x - 1) - (x^2 + 1)) / (x - 1)²
```

## Square Root Examples

### 13. Square Root
**Input:** `sqrt(x^2 + 1)`

**Steps:**
```
Step 1: Chain Rule: ∂/∂x(√u) = (1/(2√u)) ⋅ u'
Step 2: ∂/∂x(x^2 + 1) = 2 ⋅ x
Final: f'(x) = x / √(x^2 + 1)
```

## Complex Examples

### 14. Multiple Operations
**Input:** `(x^2 + sin(x))*exp(x)/ln(x)`

**Steps:**
```
Multiple rules applied:
- Quotient Rule (outer)
- Product Rule (numerator)
- Chain Rule (sin, exp, ln components)
```

### 15. Highly Nested
**Input:** `sin(exp(x^2))`

**Steps:**
```
Step 1: Chain Rule (sin): cos(exp(x^2)) ⋅ ∂/∂x(exp(x^2))
Step 2: Chain Rule (exp): exp(x^2) ⋅ ∂/∂x(x^2)
Step 3: Power Rule: 2 ⋅ x
Final: f'(x) = 2 ⋅ x ⋅ exp(x^2) ⋅ cos(exp(x^2))
```

## UTF-8 Symbol Reference

The application displays these mathematical symbols:

| Symbol | Meaning | Unicode |
|--------|---------|---------|
| ∂ | Partial derivative | U+2202 |
| ⋅ | Dot (multiplication) | U+22C5 |
| √ | Square root | U+221A |
| π | Pi | U+03C0 |
| α, β, γ | Greek letters | U+03B1-03C9 |
| ∫ | Integral | U+222B |
| ∑ | Summation | U+2211 |
| ² ³ | Superscripts | U+00B2, U+00B3 |

## Testing Your Build

Run the application and press SPACE to cycle through built-in examples. Verify:

1. ✅ All UTF-8 symbols render correctly
2. ✅ Step-by-step explanations appear
3. ✅ Blue curve shows f(x)
4. ✅ Red curve shows f'(x)
5. ✅ Scrolling works (UP/DOWN keys)
6. ✅ No font rendering errors

If any symbols appear as boxes (□), your font doesn't support that Unicode range. Try:
- DejaVuSans.ttf
- Arial Unicode MS
- Noto Sans Math
