# Test2 Implementation Guide - Quick Reference

## Overview
Test2 requires printing integers, booleans, and None. Currently only strings work.

## Changes Required (3 files)

---

### File 1: `src/Evalvisitor.h`

#### Change 1: Update Value type (line 12)

**BEFORE:**
```cpp
using Value = std::variant<std::monostate, std::string>;
```

**AFTER:**
```cpp
using Value = std::variant<
    std::monostate,  // None
    bool,            // True/False  
    int,             // Integer
    std::string      // String
>;
```

#### Change 2: Add printValue declaration (line 31, in private section)

**BEFORE:**
```cpp
private:
    // Helper to remove quotes from string literals
    std::string unquoteString(const std::string& str);
};
```

**AFTER:**
```cpp
private:
    // Helper to remove quotes from string literals
    std::string unquoteString(const std::string& str);
    // Helper to print any Value type
    void printValue(const Value& val);
};
```

---

### File 2: `src/Evalvisitor.cpp`

#### Change 1: Update visitAtom() method (replace entire function around line 74)

**BEFORE:**
```cpp
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    // Check if this is a string literal
    auto strings = ctx->STRING();
    if (!strings.empty()) {
        // Get the string literal (may have quotes)
        std::string strValue = strings[0]->getText();
        // Remove quotes
        strValue = unquoteString(strValue);
        // Return as Value
        return Value(strValue);
    }
    
    // For other atoms (NAME, NUMBER, etc.), return empty for now
    return std::any();
}
```

**AFTER:**
```cpp
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    // Check if this is a string literal
    auto strings = ctx->STRING();
    if (!strings.empty()) {
        std::string strValue = strings[0]->getText();
        strValue = unquoteString(strValue);
        return Value(strValue);
    }
    
    // Check if this is a number
    auto number = ctx->NUMBER();
    if (number) {
        std::string numText = number->getText();
        int intValue = std::stoi(numText);
        return Value(intValue);
    }
    
    // Check if this is True
    if (ctx->TRUE()) {
        return Value(true);
    }
    
    // Check if this is False
    if (ctx->FALSE()) {
        return Value(false);
    }
    
    // Check if this is None
    if (ctx->NONE()) {
        return Value(std::monostate{});
    }
    
    // For other atoms (NAME, etc.), return empty for now
    return std::any();
}
```

#### Change 2: Add printValue() helper function (add after unquoteString, around line 104)

**ADD THIS NEW FUNCTION:**
```cpp
void EvalVisitor::printValue(const Value& val) {
    if (std::holds_alternative<std::monostate>(val)) {
        std::cout << "None" << std::endl;
    } else if (std::holds_alternative<bool>(val)) {
        std::cout << (std::get<bool>(val) ? "True" : "False") << std::endl;
    } else if (std::holds_alternative<int>(val)) {
        std::cout << std::get<int>(val) << std::endl;
    } else if (std::holds_alternative<std::string>(val)) {
        std::cout << std::get<std::string>(val) << std::endl;
    }
}
```

#### Change 3: Update print function in visitAtom_expr() (around line 54-61)

**BEFORE:**
```cpp
if (argValue.has_value()) {
    try {
        Value val = std::any_cast<Value>(argValue);
        if (std::holds_alternative<std::string>(val)) {
            std::cout << std::get<std::string>(val) << std::endl;
        }
    } catch (...) {
        // Not a Value, ignore
    }
}
```

**AFTER:**
```cpp
if (argValue.has_value()) {
    try {
        Value val = std::any_cast<Value>(argValue);
        printValue(val);
    } catch (...) {
        // Not a Value, ignore
    }
}
```

---

## Build and Test

```bash
# Build
cmake -B build -S .
cd build && make -j4

# Test (should output: 65536, True, None)
./code < ../testcases/basic-testcases/test2.in

# Verify test1 still works (regression test)
./code < ../testcases/basic-testcases/test1.in
```

---

## Expected Results

### Test1 (regression - should still pass):
```
Hello, World!
```

### Test2 (should now pass):
```
65536
True
None
```

---

## Checklist

- [ ] Update Value type in Evalvisitor.h
- [ ] Add printValue() declaration in Evalvisitor.h  
- [ ] Implement visitAtom() NUMBER handling
- [ ] Implement visitAtom() TRUE handling
- [ ] Implement visitAtom() FALSE handling
- [ ] Implement visitAtom() NONE handling
- [ ] Implement printValue() helper function
- [ ] Update print function to use printValue()
- [ ] Build successfully
- [ ] Test1 passes (regression)
- [ ] Test2 passes

---

## Notes

- All changes extend existing patterns - no architectural changes needed
- Changes are localized to 2 files
- Low risk - test1 should continue to work
- Estimated time: 30-45 minutes
