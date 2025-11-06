# Quick Guide: Custom Limit Configuration

## 🎯 Two Ways to Set Limits

### Option 1: Quick Toggle (Fast)
```
Press T → Cycles through: Finite → +∞ → -∞ → Finite
```
- Instant recalculation
- Uses current point for finite limits
- Best for quick exploration

### Option 2: Full Config (Precise)
```
Press L or P → Opens configuration screen
```
- Choose any limit type
- Enter custom point value
- Full control over both type and point

---

## 📋 Configuration Screen Controls

| Key | Action |
|-----|--------|
| **L** or **P** | Open config |
| **↑↓** | Change type (Finite/+∞/-∞) |
| **TAB** | Switch between Type ↔ Point |
| **Type numbers** | Enter custom point |
| **BACKSPACE** | Delete digit |
| **ENTER** | Apply & compute |
| **ESC** | Cancel |

---

## 💡 Quick Examples

### Example 1: Custom Point
```
1. Press L
2. Keep "Finite" (or select with ↑↓)
3. Press TAB
4. Type: 5
5. Press ENTER
→ Calculates lim [x→5] f(x)
```

### Example 2: Limit at Infinity
```
1. Press L
2. Press ↓ to select "+∞"
3. Press ENTER
→ Calculates lim [x→+∞] f(x)
```

### Example 3: Quick Toggle
```
1. Press T (multiple times)
→ Cycles: Finite → +∞ → -∞
```

---

## 🎨 Screen Layout

```
Configure Limit:

> Limit Type: x → 2.000000        ← Green = Active
    [UP/DOWN to change]

  Limit Point: 2.0_               ← White = Inactive
                                    _ = Typing cursor

Controls:
  TAB - Switch fields
  UP/DOWN - Change type
  ENTER - Apply
  ESC - Cancel
```

---

## ✨ Features

✅ **Any finite point** - Type any number (e.g., 3.14, -5, 0)  
✅ **Infinity limits** - Select +∞ or -∞  
✅ **Visual navigation** - See which field is active  
✅ **Easy switching** - TAB between type and point  
✅ **Instant feedback** - Shows current configuration  

---

## 🔑 Keyboard Reference

**In Limits Mode:**
```
ENTER    → Type custom expression
L or P   → Configure limit (type + point)
T        → Quick toggle type only
SPACE    → Next example
↑↓       → Scroll results
ESC      → Back to menu
```

**In Config Screen:**
```
↑↓       → Change type
TAB      → Switch fields
Numbers  → Type point value
ENTER    → Apply
ESC      → Cancel
```

---

## 🎓 Common Use Cases

### Case 1: L'Hôpital's Rule Test
```
Expression: (x^2 - 4)/(x - 2)
Point: x → 2
→ Shows 0/0 form and applies L'Hôpital
→ Result: 4
```

### Case 2: End Behavior
```
Expression: 1/x
Limit: x → +∞
→ Result: 0
```

### Case 3: Function Analysis
```
Test same function at multiple points:
1. Set x → 0
2. Press L, TAB, type "1", ENTER → x → 1
3. Press L, TAB, type "5", ENTER → x → 5
4. Press L, ↓ twice, ENTER → x → +∞
```

---

## 🚀 Get Started

1. **Enter Limits Mode** - Press 4 from main menu
2. **Try default example** - Already loaded
3. **Press L** - Opens configuration
4. **Experiment!** - Change types and points

---

## 💡 Pro Tips

- Use **T** for quick type changes
- Use **L/P** when you need exact points
- Press **SPACE** to see more examples
- Try limits at interesting points (0, 1, infinity)
- Watch for indeterminate forms (auto L'Hôpital!)

---

## Need Help?

Full documentation: `LIMIT_CONFIG_FEATURE.md`

Enjoy exploring limits! 🎉
