# Quick Start Guide - New Features

## 🚀 Testing the New Features

### Prerequisites
You'll need to rebuild the project with the new source files. Open MSYS2 MinGW64 terminal:

```bash
cd /d/Mathh/build
cmake --build . -j 4
```

---

## 🧪 Testing Limits Calculator

### Test 1: Basic Limit with Direct Substitution
1. Launch the application: `./MathEngineUTF8.exe`
2. Press **4** to select Limits mode
3. You'll see the first example: `(x^2 - 4)/(x - 2)` as x→2
4. Observe the steps showing L'Hôpital's rule application
5. Expected result: **4**

### Test 2: Toggle Between Limit Types
1. In Limits mode, press **T** to toggle limit type
2. Watch as the limit changes from:
   - Finite (x → 2.0)
   - +∞ (x → +∞)
   - -∞ (x → -∞)
3. The result updates automatically

### Test 3: Try Different Examples
1. Press **SPACE** to cycle through examples:
   - `(x^2 - 4)/(x - 2)` → Classic L'Hôpital case
   - `sin(x)/x` → Famous limit equals 1
   - `(1 - cos(x))/x` → Trigonometric limit
   - `x^2` → Polynomial at infinity
   - `1/x` → Approaches 0 at infinity

### Test 4: Custom Expression
1. Press **ENTER** to input mode
2. Type: `(x^3 - 8)/(x - 2)`
3. Press **ENTER** to compute
4. Press **P** to change the limit point
5. Type: `2` and press **ENTER**
6. Expected: Should show L'Hôpital's rule steps → Result: 12

---

## 🧪 Testing Matrix Multiplication

### Test 1: Simple 2×2 Multiplication
1. From main menu, press **5** for Matrix Multiplication
2. Enter dimensions:
   - Matrix A rows: `2` (ENTER)
   - Press TAB
   - Matrix A cols: `2` (ENTER)
   - Press TAB
   - Matrix B rows: `2` (ENTER)
   - Press TAB
   - Matrix B cols: `2` (ENTER)
   - Press ENTER to confirm

3. Enter Matrix A values:
   ```
   [0][0]: 1
   [0][1]: 2
   [1][0]: 3
   [1][1]: 4
   ```

4. Enter Matrix B values:
   ```
   [0][0]: 5
   [0][1]: 6
   [1][0]: 7
   [1][1]: 8
   ```

5. Expected Result:
   ```
   [   19.00    22.00 ]
   [   43.00    50.00 ]
   ```

### Test 2: Non-Square Matrix (2×3) × (3×2)
1. Press **5** from menu
2. Enter dimensions:
   - A: 2 rows, 3 cols
   - B: 3 rows, 2 cols

3. Enter Matrix A:
   ```
   [0][0]: 1  [0][1]: 2  [0][2]: 3
   [1][0]: 4  [1][1]: 5  [1][2]: 6
   ```

4. Enter Matrix B:
   ```
   [0][0]: 7   [0][1]: 8
   [1][0]: 9   [1][1]: 10
   [2][0]: 11  [2][1]: 12
   ```

5. Expected Result (2×2):
   ```
   [   58.00    64.00 ]
   [  139.00   154.00 ]
   ```

### Test 3: Dimension Mismatch (Error Case)
1. Press **5** from menu
2. Enter incompatible dimensions:
   - A: 2 rows, 3 cols
   - B: 2 rows, 2 cols ← Notice B.rows ≠ A.cols

3. Complete value entry (any values)
4. Expected: Clear error message:
   ```
   Cannot multiply: A(2×3) × B(2×2)
   Number of columns in A (3) must equal number of rows in B (2)
   ```

---

## 🎯 Feature Highlights to Check

### Limits Calculator
- ✓ Direct substitution attempt
- ✓ Indeterminate form detection
- ✓ L'Hôpital's rule automatic application
- ✓ Multiple iteration support (up to 3 times)
- ✓ Step-by-step explanations
- ✓ Toggle between finite/+∞/-∞
- ✓ Custom point entry

### Matrix Multiplication
- ✓ Dimension validation
- ✓ Clear error messages
- ✓ Interactive value entry with visual feedback
- ✓ Shows current progress (which element being entered)
- ✓ Displays intermediate calculations
- ✓ Formatted result matrix
- ✓ Handles large matrices (shows first 6 steps)

---

## 🎮 Controls Reference

### Limits Mode
| Key | Action |
|-----|--------|
| ENTER | Input custom expression |
| P | Change limit point |
| T | Toggle limit type (Finite/+∞/-∞) |
| SPACE | Next example |
| UP/DOWN | Scroll through steps |
| ESC | Return to menu |

### Matrix Multiplication Mode
| Key | Action |
|-----|--------|
| TAB | Switch dimension fields |
| ENTER | Confirm value/proceed |
| BACKSPACE | Delete character |
| ESC | Cancel and return to menu |

---

## 🐛 If Build Fails

### Missing cmake
If you see "cmake is not recognized":
1. Open **MSYS2 MinGW64** terminal (not PowerShell)
2. Navigate to project: `cd /d/Mathh/build`
3. Run: `cmake --build . -j 4`

### Clean Build (if needed)
```bash
cd /d/Mathh
rm -rf build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build . -j 4
```

### Copy DLLs (if running outside MSYS2)
```bash
cd /d/Mathh/build
ldd MathEngineUTF8.exe | grep mingw64 | awk '{print $3}' | xargs -I {} cp {} .
```

---

## ✅ Success Checklist

After building, verify:
- [ ] Application launches without errors
- [ ] Menu shows options 4 and 5
- [ ] Can select Limits mode (option 4)
- [ ] Can select Matrix Multiplication mode (option 5)
- [ ] Limits show step-by-step calculations
- [ ] Matrix multiplication accepts input
- [ ] Error messages display correctly
- [ ] Can return to menu from all modes
- [ ] All original features still work (1, 2, 3)

---

## 📊 Sample Output Screenshots

### Limits Mode Output
```
Limits Mode - lim f(x)

Input: f(x) = (x^2 - 4)/(x - 2)
Limit: x → 2.00

--- Limit Calculation Steps ---
Step 1: Evaluating limit
  lim [x → 2.00] ((x^2 - 4) / (x - 2))

Step 2: Direct substitution
  Substitute x = 2.000000

Step 3: Result
  Indeterminate form (NaN)

Step 4: Indeterminate form detected
  0/0 or ∞/∞ - applying L'Hôpital's rule

Step 5: Applying L'Hôpital's rule (iteration 1)
  Differentiate numerator and denominator separately

Step 6: After differentiation
  ((2 ⋅ x) - 0) / (1 - 0)

Step 7: L'Hôpital result
  4.0000 / 1.0000 = 4.000000

--- Final Result ---
lim [x → 2.00] f(x) = 4.000000
```

### Matrix Multiplication Output
```
Matrix Multiplication Mode

--- Matrix Multiplication Steps ---
Matrix Dimensions:
  Matrix A: 2×3, Matrix B: 3×2

Validation:
  ✓ Multiplication is valid (A.cols = B.rows)

Result Dimensions:
  Result will be 2×2

Formula:
  C[i][j] = Σ(k=0 to n-1) A[i][k] × B[k][j]

Computing element [0][0]:
  (1.00 × 7.00) + (2.00 × 9.00) + (3.00 × 11.00) = 58.00

Computing element [0][1]:
  (1.00 × 8.00) + (2.00 × 10.00) + (3.00 × 12.00) = 64.00

[... additional calculations ...]

Final Result Matrix:
[    58.00     64.00 ]
[   139.00    154.00 ]
```

---

## 🎉 You're Ready!

The new features are fully integrated and ready to use. Enjoy exploring limits and matrix operations with step-by-step visualizations!

For detailed documentation, see `NEW_FEATURES.md`.
