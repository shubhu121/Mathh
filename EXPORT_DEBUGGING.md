# LaTeX Export Troubleshooting Guide

## 🔍 Quick Diagnosis

### Did you recompile after the fixes?

**You MUST recompile with the corrected files!**

```bash
cd /d/Mathh/build

g++ -std=c++17 -I/mingw64/include -I/mingw64/include/SDL2 -I../src ../src/main.cpp ../src/engine/parser.cpp ../src/engine/differentiator.cpp ../src/engine/simplifier.cpp ../src/engine/integrator.cpp ../src/engine/limit_calculator.cpp ../src/engine/matrix_operations.cpp ../src/engine/latex_exporter.cpp ../src/ui/renderer.cpp ../src/ui/text_renderer.cpp ../src/ui/plotter.cpp -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lglew32 -lopengl32 -lfreetype -lharfbuzz -lgraphite2 -lbz2 -lz -lpng -lrpcrt4 -mwindows -o MathEngineUTF8.exe
```

**Note:** Make sure you include `../src/engine/latex_exporter.cpp` in the command!

---

## ✅ Step-by-Step Test

### Test 1: Verify Compilation
```bash
# Check if executable was updated
ls -l MathEngineUTF8.exe
# Should show recent timestamp
```

### Test 2: Run and Check Console Output
```bash
./MathEngineUTF8.exe
```

When you press X, you should see in the **console/terminal**:
```
LaTeX file saved: differentiation_solution.tex
Attempting PDF compilation...
PDF compilation failed (pdflatex not found or error occurred)
```

### Test 3: Verify File Location
After pressing X, check for files:
```bash
ls -la *.tex
ls -la *.pdf
```

Files are created in the **current working directory** (usually `build/`).

---

## 🐛 Common Issues

### Issue 1: "Nothing happens when I press X"

**Cause:** No solution has been computed yet

**Solution:**
1. Enter a mode (press 1-5)
2. **Wait for solution to be displayed**
3. **Then** press X

**Example:**
```
1. Press 1 (Differentiation)
2. See the solution displayed on screen
3. NOW press X
4. Check console output
```

### Issue 2: "Export failed - no solution to export"

This appears when:
- You're in MENU mode (export disabled)
- No problem has been solved yet
- Solution failed to compute

**Solution:** Solve a problem first, then export.

### Issue 3: "Files not found"

**Check current directory:**
```bash
pwd
# Should be: /d/Mathh/build

# List all files
ls -la
```

**Files are created in build directory**, not src directory!

### Issue 4: "X key doesn't trigger anything"

**Check if you're in the right mode:**
- ❌ X doesn't work in **MENU** mode
- ✅ X works in **Differentiation** mode
- ✅ X works in **Integration** modes
- ✅ X works in **Limits** mode
- ✅ X works in **Matrix** mode

### Issue 5: "PDF not created"

This is **NORMAL** if you don't have pdflatex installed!

**The .tex file is still created** - check for it:
```bash
ls *.tex
```

**To get PDF:** Install pdflatex (optional):
```bash
# Install MiKTeX or TeX Live
# See LATEX_EXPORT_FEATURE.md for details
```

---

## 🔬 Advanced Debugging

### Check if LaTeX exporter is linked

```bash
# In build directory
strings MathEngineUTF8.exe | grep "Exported to"
```

Should show: "Exported to "

### Add Debug Output

Edit `src/main.cpp`, find the X key handler (around line 764):

```cpp
case SDLK_x:
    // X key to export solution to LaTeX
    std::cout << "X key pressed! Mode: " << (int)currentMode << std::endl;
    std::cout << "parseSuccess: " << parseSuccess << std::endl;
    if (currentMode != Mode::MENU) {
        exportToLaTeX();
    }
    break;
```

Recompile and run - you'll see debug output when X is pressed.

---

## 📝 Expected Workflow

### Correct Usage:
```
1. Launch: ./MathEngineUTF8.exe
2. Press 1 (Differentiation mode)
3. See solution displayed:
   "Input: f(x) = sin(x^2)"
   "--- Differentiation Steps ---"
   "f'(x) = ..."
4. Press X
5. Console shows:
   "LaTeX file saved: differentiation_solution.tex"
6. Green message appears on screen:
   "Exported to differentiation_solution.tex"
7. Check build/ folder:
   $ ls *.tex
   differentiation_solution.tex
```

### What You Should See:

**On Screen:**
- Green message at bottom: "Exported to [filename]"
- Message appears for 3 seconds

**In Console:**
```
LaTeX file saved: differentiation_solution.tex
Attempting PDF compilation...
PDF compilation failed (pdflatex not found or error occurred)
```

**In Build Directory:**
```bash
$ ls *.tex
differentiation_solution.tex
```

---

## 🧪 Quick Test Commands

Run these in sequence:

```bash
# 1. Navigate to build
cd /d/Mathh/build

# 2. Recompile with LaTeX exporter
g++ -std=c++17 -I/mingw64/include -I/mingw64/include/SDL2 -I../src ../src/main.cpp ../src/engine/parser.cpp ../src/engine/differentiator.cpp ../src/engine/simplifier.cpp ../src/engine/integrator.cpp ../src/engine/limit_calculator.cpp ../src/engine/matrix_operations.cpp ../src/engine/latex_exporter.cpp ../src/ui/renderer.cpp ../src/ui/text_renderer.cpp ../src/ui/plotter.cpp -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lglew32 -lopengl32 -lfreetype -lharfbuzz -lgraphite2 -lbz2 -lz -lpng -lrpcrt4 -mwindows -o MathEngineUTF8.exe

# 3. Run
./MathEngineUTF8.exe

# In the app:
# - Press 1 (Differentiation)
# - Wait for solution to display
# - Press X
# - Watch console output

# 4. In another terminal, check for files
cd /d/Mathh/build
ls -l *.tex
cat differentiation_solution.tex
```

---

## 📊 Checklist

Before asking for help, verify:

- [ ] Recompiled with `latex_exporter.cpp` included
- [ ] Executable timestamp is recent (after recompile)
- [ ] Entered a calculation mode (not in menu)
- [ ] Solution is displayed on screen
- [ ] Pressed X key (not x in text input!)
- [ ] Checked console output for messages
- [ ] Looked in build/ directory for .tex files
- [ ] Checked if green message appears on screen

---

## 💡 Most Likely Issue

**You probably need to recompile!**

The previous compilation didn't include `latex_exporter.cpp`, so the export functionality isn't in your executable.

**Solution:**
```bash
cd /d/Mathh/build
# Copy the FULL command from above and run it
# Make sure latex_exporter.cpp is in the file list!
```

---

## 🆘 Still Not Working?

If you've done all the above and it still doesn't work:

1. **Show me your console output** when you press X
2. **Run:** `ls -la` in build directory
3. **Check:** Does the green message appear on screen?
4. **Verify:** `strings MathEngineUTF8.exe | grep "LaTeX"`

The export feature is definitely in the code - if it's not working, it's likely a compilation or runtime issue!
