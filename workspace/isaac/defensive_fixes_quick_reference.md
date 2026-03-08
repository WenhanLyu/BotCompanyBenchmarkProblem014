# Defensive Fixes Quick Reference
## Priority P0 - Critical Crash Prevention

### Fix 1: INT_MIN Negation Overflow
**File:** `src/Evalvisitor.cpp`  
**Line:** 889-890  
**Risk:** Undefined behavior on `-INT_MIN`

**Current code:**
```cpp
if (std::holds_alternative<int>(factorVal)) {
    return Value(-std::get<int>(factorVal));  // CRASH if factorVal == INT_MIN
}
```

**Fixed code:**
```cpp
if (std::holds_alternative<int>(factorVal)) {
    int val = std::get<int>(factorVal);
    if (val == INT_MIN) {
        // Promote to BigInteger to avoid overflow
        return Value(-BigInteger(val));
    }
    return Value(-val);
}
```

**Test case:**
```python
x = -2147483648
y = -x
print(y)  # Should print 2147483648, not crash
```

---

### Fix 2: String Repetition Negative Count
**File:** `src/Evalvisitor.cpp`  
**Lines:** 175-186  
**Risk:** Undefined behavior with negative reserve

**Current code:**
```cpp
std::string s = std::get<std::string>(currentValue);
int count = std::get<int>(rightValue);
std::string repeated;
repeated.reserve(s.size() * count);  // CRASH if count < 0
for (int i = 0; i < count; i++) {
    repeated.append(s);
}
result = repeated;
```

**Fixed code:**
```cpp
std::string s = std::get<std::string>(currentValue);
int count = std::get<int>(rightValue);
if (count <= 0) {
    // Python: negative or zero count → empty string
    result = std::string("");
} else {
    std::string repeated;
    // Optional: check for overflow in allocation
    if (s.size() > 0 && count > INT_MAX / s.size()) {
        throw std::runtime_error("String repetition result too large");
    }
    repeated.reserve(s.size() * count);
    for (int i = 0; i < count; i++) {
        repeated.append(s);
    }
    result = repeated;
}
```

**Test case:**
```python
s = "abc" * -5
print(s)  # Should print empty string, not crash
print(len(s))  # Should print 0
```

---

### Fix 3: Exception Handling Format
**File:** `src/main.cpp`  
**Lines:** 18-24  
**Risk:** Wrong output format for OJ

**Current code:**
```cpp
try {
    visitor.visit(tree);
} catch (const std::exception& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
    return 1;
}
```

**Option A - Silent (Recommended for OJ):**
```cpp
try {
    visitor.visit(tree);
} catch (const std::exception&) {
    // Silent failure - clean exit
    // OJ may expect no output on error
}
return 0;
```

**Option B - Python-style (If OJ expects error message):**
```cpp
try {
    visitor.visit(tree);
} catch (const std::runtime_error& e) {
    std::string msg = e.what();
    if (msg.find("Division by zero") != std::string::npos || 
        msg.find("Modulo by zero") != std::string::npos) {
        std::cout << "ZeroDivisionError: integer division or modulo by zero" << std::endl;
    } else {
        std::cout << "RuntimeError: " << msg << std::endl;
    }
} catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
}
return 0;
```

**Decision:** Start with Option A. If tests 34, 55, 72 show Wrong Answer, try Option B.

---

### Fix 4: Augmented Assignment Result Init
**File:** `src/Evalvisitor.cpp`  
**Line:** 98  
**Risk:** Undefined behavior if no operator matches

**Current code:**
```cpp
Value result;  // Uninitialized

if (op == "+=") {
    // ...
} else if (op == "-=") {
    // ...
}
// ... more operators

// Later: uses result without checking if it was set
(*localVariables)[varName] = result;
```

**Fixed code:**
```cpp
Value result = std::monostate{};  // Initialize to None

if (op == "+=") {
    // ...
} else if (op == "-=") {
    // ...
} else {
    // Defensive: should never reach here
    throw std::runtime_error("Unsupported augmented assignment operator: " + op);
}

// Safe to use result now
(*localVariables)[varName] = result;
```

---

## Priority P1 - Correctness Issues

### Fix 5: Type Coercion Helper
**New function to add to Evalvisitor.cpp**

```cpp
// Add to private section of EvalVisitor class
double valueToDouble(const Value& val) {
    if (std::holds_alternative<double>(val)) {
        return std::get<double>(val);
    }
    if (std::holds_alternative<int>(val)) {
        return static_cast<double>(std::get<int>(val));
    }
    if (std::holds_alternative<bool>(val)) {
        return std::get<bool>(val) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<BigInteger>(val)) {
        return std::stod(std::get<BigInteger>(val).toString());
    }
    throw std::runtime_error("Cannot convert value to double");
}
```

**Replace all instances like:**
```cpp
double left = std::holds_alternative<double>(currentValue) ? 
              std::get<double>(currentValue) : 
              static_cast<double>(std::get<int>(currentValue));
```

**With:**
```cpp
double left = valueToDouble(currentValue);
```

---

### Fix 6: Double Floor Division
**File:** `src/Evalvisitor.cpp`  
**Lines:** 215, 856, 858  
**Risk:** Precision loss

**Current code:**
```cpp
result = pythonFloorDiv(static_cast<int>(left), static_cast<int>(right));
```

**Fixed code:**
```cpp
// For double floor division, use proper floor
result = std::floor(left / right);
```

---

## Testing Checklist

After applying fixes, test these cases:

```bash
# Build
make clean && make

# Test 1: INT_MIN negation
echo "x = -2147483648
y = -x
print(y)" | ./code
# Expected: 2147483648

# Test 2: Negative string repetition
echo 's = "abc" * -5
print(s)
print(len(s))' | ./code
# Expected: (empty line), 0

# Test 3: Division by zero
echo "print(10 // 0)" | ./code
echo $?
# Expected: exit code 0 (no crash)

# Test 4: Zero division in variable
echo "x = 0
y = 10 // x
print(y)" | ./code
# Expected: clean exit

# Test 5: Verify no regression
./code < testcases/basic-testcases/test1.in
# Expected: 65536, True, None
```

---

## Implementation Order

1. **Fix exception handling** (main.cpp) - 2 minutes
2. **Fix INT_MIN negation** (Evalvisitor.cpp:889) - 5 minutes
3. **Fix string repetition** (Evalvisitor.cpp:175) - 5 minutes
4. **Test all fixes** - 5 minutes
5. **Fix augmented assignment init** (Evalvisitor.cpp:98) - 2 minutes
6. **Add type coercion helper** (if time permits) - 15 minutes
7. **Fix double floor division** (if time permits) - 5 minutes

**Total time for P0 fixes:** ~20 minutes  
**Total time for P0+P1:** ~40 minutes

---

## Verification

After fixes:
1. Run local tests: `./test_m1.sh`
2. Check no crashes on edge cases above
3. Verify exit codes are 0
4. Commit with message: `[Isaac] Add defensive fixes for edge cases`
5. Submit to OJ and check tests 34, 55, 72

Expected improvement: 3 tests (possibly more if other tests hit these edge cases)
