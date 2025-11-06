# Quick LaTeX Export Guide

## 🚀 One-Minute Start

### Export Any Solution
```
1. Solve problem → Press X
2. ✓ LaTeX file saved
3. ✓ PDF created (if pdflatex installed)
```

### Files Created
- `differentiation_solution.tex` + `.pdf`
- `integration_solution.tex` + `.pdf`
- `definite_integration_solution.tex` + `.pdf`
- `limit_solution.tex` + `.pdf`
- `matrix_multiplication_solution.tex` + `.pdf`

---

## 📋 Quick Examples

### Example 1: Export Derivative
```
1. Press 1 (Differentiation)
2. Solve: sin(x^2)
3. Press X
→ differentiation_solution.pdf created!
```

### Example 2: Export Integral
```
1. Press 2 (Indefinite Integration)
2. Solve: x^2
3. Press X
→ integration_solution.pdf created!
```

### Example 3: Export Limit
```
1. Press 4 (Limits)
2. Solve: (x^2-4)/(x-2) as x→2
3. Press X
→ limit_solution.pdf created!
```

---

## 💻 Install pdflatex (Optional but Recommended)

### Windows - MiKTeX
```
Download: https://miktex.org/download
Or: choco install miktex
```

###Windows - TeX Live
```
Download: https://www.tug.org/texlive/
```

### Linux
```bash
sudo apt-get install texlive-latex-base texlive-latex-extra
```

### macOS
```bash
brew install --cask mactex
```

---

## 📄 Using Exported Files

### Option 1: Use the PDF Directly
- Open `.pdf` file
- Print or share
- Professional formatting included

### Option 2: Copy LaTeX Code
1. Open `.tex` file in text editor
2. Copy the parts you need
3. Paste into Overleaf, thesis, homework, etc.

### Option 3: Modify the LaTeX
1. Open `.tex` in text editor
2. Edit as needed
3. Recompile: `pdflatex yourfile.tex`

---

## 🎓 Common Uses

### For Homework
```
1. Solve problems in engine
2. Export each solution (X key)
3. Copy LaTeX into homework doc
4. Submit professionally formatted work
```

### For Study Notes
```
1. Solve practice problems
2. Export solutions
3. Combine PDFs or LaTeX code
4. Create comprehensive study guide
```

### For Presentations
```
1. Solve example problems
2. Export to LaTeX
3. Include in Beamer slides
4. Professional mathematical notation
```

---

## 🔑 Key Points

✅ **X key** exports current solution  
✅ Works in **all modes** (except menu)  
✅ **Auto-compiles** to PDF if pdflatex available  
✅ LaTeX file **always created** (even without pdflatex)  
✅ **Overwrites** previous export of same type  
✅ Saved in **build directory**  

---

## 🐛 If Export Doesn't Work

### "Export failed"
→ Solve a problem first, then press X

### "PDF not created"
→ Install pdflatex (see above)  
→ LaTeX file still created - compile manually

### "Can't find exported file"
→ Check `d:\Mathh\build\` directory  
→ Look for `*_solution.tex` files

---

## 📚 LaTeX Notation Examples

| You See | LaTeX Creates |
|---------|---------------|
| `sin(x)` | `\sin(x)` |
| `x^2` | `x^{2}` |
| `a/b` | `\frac{a}{b}` |
| `sqrt(x)` | `\sqrt{x}` |
| `∫ f(x) dx` | `\int f(x) \, dx` |
| `lim x→∞` | `\lim_{x \to \infty}` |

---

## 🎯 Pro Tips

1. **Rename files** if you want to keep multiple solutions
2. **Check build folder** for all exported files
3. **Install pdflatex** for automatic PDF creation
4. **Copy LaTeX code** to customize formatting
5. **Use Overleaf** for easy online editing

---

## 🎉 That's It!

**Press X to export. It's that simple!**

For detailed documentation, see `LATEX_EXPORT_FEATURE.md`
