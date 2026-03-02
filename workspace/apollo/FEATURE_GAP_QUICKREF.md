# Feature Gap Quick Reference

## Critical Issues (MUST FIX for OJ)

### 1. F-Strings (test14-15) ❌ BROKEN

**Current Behavior**: Evaluates to `0`  
**Expected Behavior**: Should produce formatted strings

**Example**:
```python
print(f"Number is { 42 }")
# Current output: 0
# Expected output: Number is 42
```

**What's Needed**:
- Fix expression evaluation inside `{...}`
- Convert evaluated values to strings
- Concatenate string parts with evaluated expressions
- Handle nested f-strings: `f"{ f"inner" } outer"`

---

### 2. Global Keyword (test13) ❌ NOT IMPLEMENTED

**Current Behavior**: UnboundLocalError or infinite loop  
**Expected Behavior**: Should allow function to modify global variables

**Example**:
```python
count = 0

def increment():
    global count  # <-- This keyword is missing
    count += 1

increment()
print(count)  # Should print 1
```

**What's Needed**:
- Add `global` keyword to lexer
- Parse global declarations in functions
- Track which variables are global per function scope
- Allow write access to global scope when declared

---

## Optional/Future (Check OJ Requirements)

### 3. BigInteger (BigIntegerTest0-19) ❌ NOT IMPLEMENTED

**Current Behavior**: Cannot handle integers > ~19 digits  
**Expected Behavior**: Should support arbitrary precision integers

**Example**:
```python
a = 123456789012345678901234567890  # 30 digits
b = 987654321098765432109876543210  # 30 digits
print(a + b)  # Should work with any size
```

**Complexity**: VERY HIGH (40-80 hours)  
**Alternative**: Use GMP library (GNU Multiple Precision)

---

## Test Pass Rate

| Category | Status | Count |
|----------|--------|-------|
| Basic features (test0-12) | ✅ PASS | 13/13 |
| Global keyword (test13) | ❌ FAIL | 0/1 |
| F-strings (test14-15) | ❌ FAIL | 0/2 |
| BigInteger (BigIntegerTest0-19) | ❌ FAIL | 0/20 |
| **TOTAL** | **36.1%** | **13/36** |

---

## Priority Order

1. **P1**: Fix F-strings (2 tests) - 8-16 hours
2. **P2**: Implement global keyword (1 test) - 4-8 hours  
3. **P3**: Assess BigInteger requirements (20 tests) - Check OJ first

---

## Quick Diagnostic Commands

```bash
# Test f-strings
echo 'print(f"Result: { 1 + 1 }")' | ./code
# Expected: Result: 2
# Actual: 0

# Test global keyword
echo 'x = 5
def f():
    global x
    x = 10
f()
print(x)' | ./code
# Expected: 10
# Actual: Error or wrong value

# Test BigInteger
echo 'print(99999999999999999999 + 1)' | ./code
# Expected: 100000000000000000000
# Actual: Overflow or wrong value
```
