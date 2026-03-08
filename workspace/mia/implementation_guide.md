# Implementation Guide for Missing Features

**Quick reference for developers assigned to implement the identified gaps**

---

## 1. Break Statement

### Current Issue
```python
while i < 10:
    if i == 3:
        break  # Ignored - loop continues
    i += 1
```

### Implementation Pattern
Follow the ReturnException pattern in `EvalVisitor.h`:

```cpp
// In EvalVisitor.h
class BreakException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Break statement executed";
    }
};

// Add visitor method declaration:
std::any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override;
```

```cpp
// In EvalVisitor.cpp
std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
    throw BreakException();
}

// In visitWhile_stmt(), wrap suite execution:
try {
    visit(ctx->suite());
} catch (const BreakException&) {
    break;  // Exit while loop
}
```

**Estimated LOC:** 50-80  
**Risk:** LOW

---

## 2. Continue Statement

### Current Issue
```python
while i < 5:
    i += 1
    if i == 3:
        continue  # Ignored - doesn't skip to next iteration
    print(i)
```

### Implementation Pattern
Similar to break:

```cpp
// In EvalVisitor.h
class ContinueException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Continue statement executed";
    }
};

// Add visitor method:
std::any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override;
```

```cpp
// In EvalVisitor.cpp
std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
    throw ContinueException();
}

// In visitWhile_stmt():
while (valueToBool(condition)) {
    try {
        visit(ctx->suite());
    } catch (const BreakException&) {
        break;
    } catch (const ContinueException&) {
        continue;  // Skip to next iteration
    }
}
```

**Estimated LOC:** 50-80  
**Risk:** LOW

---

## 3. Type Conversion Functions

### Current Issue
```python
x = int(3.14)  # Returns 0 instead of 3
y = float("3.5")  # Returns 0 instead of 3.5
```

### Implementation Location
Extend `visitAtom_expr()` to recognize built-in function names.

### Grammar Reference (Section 11)
- `int(x)`: Convert float/bool/str to int
- `float(x)`: Convert int/bool/str to float
- `str(x)`: Convert int/float/bool to str
- `bool(x)`: Convert int/float/str to bool
  - For str: `""` → False, any other → True

### Implementation Sketch
```cpp
// In visitAtom_expr(), after checking for user functions:
std::string funcName = atom->getText();

if (funcName == "int") {
    // Get argument
    auto arg = evaluateArgument(trailer);
    
    // Convert based on type
    if (std::holds_alternative<double>(arg)) {
        return Value((int)std::get<double>(arg));
    } else if (std::holds_alternative<bool>(arg)) {
        return Value(std::get<bool>(arg) ? 1 : 0);
    } else if (std::holds_alternative<std::string>(arg)) {
        return Value(std::stoi(std::get<std::string>(arg)));
    } else if (std::holds_alternative<BigInteger>(arg)) {
        // Already an integer type
        return arg;
    }
    // ... handle int case
}
// Similar for float(), str(), bool()
```

**Estimated LOC:** 220-360 (4 functions × 50-90 LOC each)  
**Risk:** LOW (well-specified in grammar)

---

## 4. Keyword Arguments

### Current Issue
```python
def foo(a, b):
    print(a, b)

foo(b=2, a=1)  # Prints: None None (should print: 1 2)
```

### Implementation Requirements
- Parse argument list in function call
- Match keyword arguments to parameter names
- Positional arguments must come before keyword arguments

### Grammar Reference (Section 10.2)
- `foo(1, 2)` - positional
- `foo(a=1, b=2)` - keyword
- `foo(1, b=2)` - mixed (positional first)

### Implementation Sketch
```cpp
// In visitAtom_expr() when processing function calls:
// Current: only handles positional args

// NEW: Parse arglist to separate positional and keyword args
struct Argument {
    std::string name;  // Empty for positional
    Value value;
};

std::vector<Argument> args = parseArguments(trailer);

// Match to parameters
for (size_t i = 0; i < func.parameters.size(); i++) {
    std::string paramName = func.parameters[i];
    
    // First, check keyword arguments
    auto kwIt = std::find_if(args.begin(), args.end(), 
        [&](const Argument& a) { return a.name == paramName; });
    
    if (kwIt != args.end()) {
        locals[paramName] = kwIt->value;
    } else if (i < positionalArgs.size()) {
        locals[paramName] = positionalArgs[i];
    }
    // else: use default value (M10.3 part 2)
}
```

**Estimated LOC:** 150-250  
**Risk:** MEDIUM (complex parameter matching logic)

---

## 5. Default Parameter Values

### Current Issue
```python
def foo(a=5):
    print(a)

foo()  # Prints: None (should print: 5)
```

### Implementation Requirements
- Parse default values in function definition
- Store defaults in FunctionDef struct
- Use default when argument not provided

### Grammar Reference (Section 10.1)
> "Some variables can have default values, but they must all appear after variables without default values"

### Implementation Sketch
```cpp
// Extend FunctionDef struct in EvalVisitor.h:
struct FunctionDef {
    std::vector<std::string> parameters;
    std::map<std::string, Value> defaultValues;  // NEW
    Python3Parser::SuiteContext* body;
    // ...
};

// In visitFuncdef():
// Parse parameter list and extract defaults
// Store in func.defaultValues[paramName] = defaultValue

// In function call (visitAtom_expr):
for (auto& param : func.parameters) {
    if (/* not provided in call */) {
        auto defaultIt = func.defaultValues.find(param);
        if (defaultIt != func.defaultValues.end()) {
            locals[param] = defaultIt->second;
        } else {
            locals[param] = Value(std::monostate{});  // None
        }
    }
}
```

**Estimated LOC:** 100-150  
**Risk:** MEDIUM (must integrate with keyword args)

---

## 6. Subscript Operator

### Current Issue
```python
s = "hello"
print(s[0])  # Parse error
```

### Implementation Requirements
- Handle subscript in atom_expr (atom trailer where trailer is '[' ... ']')
- Support negative indexing (Python standard)
- Bounds checking

### Grammar Reference (Section 2)
> **Subscript Operator**: subscript access `[]`

### Implementation Sketch
```cpp
// In visitAtom_expr():
auto trailers = ctx->trailer();
for (auto trailer : trailers) {
    if (trailer->getText().starts_with("[")) {
        // Subscript access
        auto indexExpr = trailer->subscriptlist();
        auto indexValue = visit(indexExpr);
        int index = std::get<int>(indexValue);
        
        if (std::holds_alternative<std::string>(currentValue)) {
            std::string str = std::get<std::string>(currentValue);
            
            // Handle negative indexing
            if (index < 0) index += str.length();
            
            // Bounds check
            if (index < 0 || index >= str.length()) {
                throw std::runtime_error("Index out of range");
            }
            
            currentValue = Value(std::string(1, str[index]));
        }
        // Could extend to lists/tuples later
    }
}
```

**Estimated LOC:** 120-200  
**Risk:** MEDIUM (negative indexing, error handling)

---

## Testing Strategy

### For Each Feature
1. Create simple test case
2. Test with interpreter: `echo 'test code' | ./code /dev/stdin`
3. Compare with Python 3: `echo 'test code' | python3`
4. Run full local test suite: `./test_m1.sh`
5. Check for regressions

### Test Cases to Create

**Break:**
```python
i = 0
while i < 10:
    if i == 3:
        break
    print(i)
    i += 1
# Expected: 0 1 2
```

**Continue:**
```python
i = 0
while i < 5:
    i += 1
    if i == 3:
        continue
    print(i)
# Expected: 1 2 4 5
```

**Type Conversions:**
```python
print(int(3.14))      # 3
print(float(5))       # 5.000000
print(str(42))        # 42
print(bool(""))       # False
print(bool("hi"))     # True
```

**Keyword Args:**
```python
def foo(a, b):
    print(a, b)
foo(b=2, a=1)  # 1 2
```

**Default Params:**
```python
def bar(x=10):
    print(x)
bar()    # 10
bar(20)  # 20
```

**Subscript:**
```python
s = "hello"
print(s[0])    # h
print(s[-1])   # o
```

---

## Reference Files

- **Grammar:** `docs/grammar.md`
- **Current implementation:** `src/EvalVisitor.cpp`, `src/EvalVisitor.h`
- **Exception pattern:** See `ReturnException` in `EvalVisitor.h` (line 23-32)
- **While loop handling:** `visitWhile_stmt()` in `EvalVisitor.cpp`
- **Function calls:** `visitAtom_expr()` in `EvalVisitor.cpp` (line 354+)

---

## Estimated Total Work

| Milestone | Features | LOC | Cycles |
|-----------|----------|-----|--------|
| M10.1 | break, continue | 100-160 | 2 |
| M10.2 | int/float/str/bool | 220-360 | 2 |
| M10.3 | keyword args, defaults | 250-400 | 3 |
| M10.4 | subscript, edge cases | 170-300 | 2 |
| **Total** | 10 features | **740-1220** | **9** |
