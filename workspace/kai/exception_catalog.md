# Exception Catalog - All Throw Statements

## Summary
The codebase contains **8 throw statements** across 2 files. ALL are uncaught.

## By File

### src/BigInteger.cpp (5 throws)

#### 1. Invalid String Constructor
**Line 30:**
```cpp
throw std::invalid_argument("Invalid number string: " + str);
```
**Thrown when:** BigInteger constructor receives invalid number string  
**Called from:** `Evalvisitor.cpp:342` when parsing large integer literals  
**Trigger:** Malformed number in input (unlikely in OJ tests)

#### 2. Division Abs by Zero
**Line 255:**
```cpp
throw std::runtime_error("Division by zero");
```
**Thrown when:** BigInteger division helper detects divisor is zero  
**Called from:** Internal helper for `operator/` and `floorDiv()`  
**Trigger:** Any BigInteger division by zero

#### 3. Floor Division by Zero
**Line 451:**
```cpp
throw std::runtime_error("Division by zero");
```
**Thrown when:** BigInteger floor division with zero divisor  
**Called from:** `Evalvisitor.cpp:611, 642` for `//` operator  
**Trigger:** `big_num // 0`

#### 4. Modulo by Zero
**Line 475:**
```cpp
throw std::runtime_error("Modulo by zero");
```
**Thrown when:** BigInteger modulo with zero divisor  
**Called from:** `Evalvisitor.cpp:613, 650` for `%` operator  
**Trigger:** `big_num % 0`

### src/Evalvisitor.cpp (3 throws)

#### 5. Integer Floor Division by Zero
**Line 1186:**
```cpp
throw std::runtime_error("Division by zero");
```
**Function:** `pythonFloorDiv(int a, int b)`  
**Called from:** Lines 131, 644, 662 for `//` operator on integers  
**Trigger:** `int_num // 0`

#### 6. Integer Modulo by Zero
**Line 1205:**
```cpp
throw std::runtime_error("Modulo by zero");
```
**Function:** `pythonModulo(int a, int b)`  
**Called from:** Lines 147, 652, 664 for `%` operator on integers  
**Trigger:** `int_num % 0`

## Call Graph

```
main.cpp:visitor.visit()  [NO TRY-CATCH]
  └─> Evalvisitor.cpp
       ├─> visitExpr_stmt (augmented assignment)
       │    ├─> Line 131: pythonFloorDiv() → THROW (line 1186)
       │    └─> Line 147: pythonModulo() → THROW (line 1205)
       │
       ├─> visitTerm (multiplication/division)
       │    ├─> Line 611: BigInteger.floorDiv() → THROW (BigInteger.cpp:451)
       │    ├─> Line 613: BigInteger.operator%() → THROW (BigInteger.cpp:475)
       │    ├─> Line 642: BigInteger.floorDiv() → THROW (BigInteger.cpp:451)
       │    ├─> Line 644: pythonFloorDiv() → THROW (line 1186)
       │    ├─> Line 650: BigInteger.operator%() → THROW (BigInteger.cpp:475)
       │    ├─> Line 652: pythonModulo() → THROW (line 1205)
       │    ├─> Line 662: pythonFloorDiv() → THROW (line 1186)
       │    └─> Line 664: pythonModulo() → THROW (line 1205)
       │
       └─> visitAtom (number parsing)
            └─> Line 342: BigInteger(string) → THROW (BigInteger.cpp:30)
```

## Test Scenarios That Will Crash

### Scenario 1: Simple Integer Division by Zero
```python
print(10 // 0)
```
**Exception Path:** visitTerm → pythonFloorDiv → throw at line 1186

### Scenario 2: Simple Integer Modulo by Zero
```python
print(5 % 0)
```
**Exception Path:** visitTerm → pythonModulo → throw at line 1205

### Scenario 3: BigInteger Division by Zero
```python
x = 99999999999999999999
print(x // 0)
```
**Exception Path:** visitTerm → BigInteger.floorDiv → throw at line 451

### Scenario 4: Augmented Assignment Division by Zero
```python
x = 10
x //= 0
```
**Exception Path:** visitExpr_stmt → pythonFloorDiv → throw at line 1186

### Scenario 5: Variable Division by Zero (Runtime)
```python
x = 10
y = 5 - 5  # y becomes 0
print(x // y)
```
**Exception Path:** visitTerm → pythonFloorDiv → throw at line 1186

### Scenario 6: Complex Expression
```python
a = 100
b = 0
c = (a // b) + 5  # Crashes before addition
```
**Exception Path:** visitTerm → pythonFloorDiv → throw at line 1186

## Potential std::bad_variant_access Sources

While not explicit throw statements, these `std::get<T>()` calls can throw if type doesn't match:

### Unchecked std::get Calls

**Line 109:** String multiplication with non-int count
```cpp
std::string s = std::get<std::string>(currentValue);  // OK, checked at line 109
int count = std::get<int>(rightValue);  // UNCHECKED if rightValue is BigInteger/None
```

**Line 657-658:** Mixed type operations
```cpp
double left = std::holds_alternative<double>(result) ? std::get<double>(result) : 
              static_cast<double>(std::get<int>(result));  // UNCHECKED if result is BigInteger
```

**Trigger:** Operations like `"hello" *= BigInteger(5)` or `None // 5`

## Python Behavior Comparison

In Python, division by zero raises `ZeroDivisionError`:
```python
>>> 10 // 0
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ZeroDivisionError: integer division or modulo by zero
```

Our current behavior: **Program terminates with SIGABRT** (uncaught exception)

## Fix Impact Analysis

### With main.cpp Exception Handling

**Caught exceptions:**
- All 6 division/modulo by zero throws
- 1 invalid BigInteger string throw
- Any potential std::bad_variant_access

**Result:**
- No more SIGABRT crashes
- Tests 34, 55, 72 will complete execution
- Whether they PASS depends on:
  - Expected output (empty, error message, or something else)
  - Behavior before the exception

### Test Outcome Scenarios

**Scenario A:** Test expects clean exit (no output)
- **Fix:** Catch and suppress → Test PASSES ✅

**Scenario B:** Test expects Python error message
- **Fix:** Catch and print "ZeroDivisionError: ..." → Test PASSES ✅

**Scenario C:** Test expects computation to continue somehow
- **Fix:** Catch and handle gracefully → Test may still FAIL ❌
- **Requires:** More sophisticated error handling (rare in interpreters)

**Scenario D:** Division by zero is not supposed to happen (bug in test input)
- **Fix:** Reveals actual test requirements → May expose other bugs ⚠️

## Recommendation Priority

1. **P0 - CRITICAL:** Add exception handling to main.cpp
2. **P1 - Optional:** Match Python error message format
3. **P2 - Future:** Add pre-check validation instead of throwing
4. **P3 - Defensive:** Add type checks before std::get calls

## Code Change Required

**Minimal fix (5 lines):**
```cpp
try {
    // existing code
} catch (...) {
    // silent exit
}
```

**Complete fix (15 lines):**
```cpp
try {
    // existing code
} catch (const std::runtime_error& e) {
    std::cerr << "ZeroDivisionError: " << e.what() << std::endl;
} catch (const std::invalid_argument& e) {
    std::cerr << "ValueError: " << e.what() << std::endl;
} catch (const std::bad_variant_access& e) {
    std::cerr << "TypeError: " << e.what() << std::endl;
} catch (...) {
    std::cerr << "Error: Unknown exception" << std::endl;
}
```
