# F-String Bug Analysis

## Root Cause Identified

**File**: `src/Evalvisitor.cpp`  
**Issue**: Missing implementation of `visitFormat_string()` method

## Evidence

### Grammar Definition (resources/Python3Parser.g4)

```antlr
atom: (NAME | NUMBER | STRING+| 'None' | 'True' | 'False' | ('(' test ')') | format_string);
format_string: FORMAT_QUOTATION (FORMAT_STRING_LITERAL | '{' testlist '}')* QUOTATION;
```

The grammar correctly defines f-strings as `format_string` with:
- `FORMAT_QUOTATION` - the `f"` or `f'` prefix
- Zero or more of:
  - `FORMAT_STRING_LITERAL` - literal text between expressions
  - `'{' testlist '}'` - expressions to be evaluated
- `QUOTATION` - closing quote

### Missing Implementation

**Search for visitor method**:
```bash
$ grep -n "visitFormat_string" src/Evalvisitor.cpp
(no results)
```

**The method is completely missing!**

This means when the parser encounters an f-string, it calls `visitFormat_string()`, which:
1. Is not overridden in EvalVisitor
2. Falls back to default ANTLR visitor behavior
3. Returns an empty or default value
4. Gets converted to integer `0`

### Test Cases Confirming Bug

```bash
$ echo 'print(f"Test: { 42 }")' | ./code
0

$ echo 'x = f"hello { 5 }"
print(x)' | ./code
0
```

The f-string is being parsed (no syntax error), but evaluation returns `0`.

### Additional Evidence: F-strings without expressions

```bash
$ echo 'print(f"Test")' | ./code
line 1:8 no viable alternative at input 'f"Test'
line 1:8 extraneous input 'Test' expecting {FORMAT_STRING_LITERAL, '{', QUOTATION}
line 1:13 no viable alternative at input ')'
```

F-strings **must** have at least one expression `{...}` in the current grammar, or they fail to parse.

## Fix Required

Implement `visitFormat_string()` in `src/Evalvisitor.cpp`:

### Pseudocode

```cpp
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
    std::string result = "";
    
    // Iterate through children of format_string
    for (auto child : ctx->children) {
        if (auto literal = dynamic_cast<FORMAT_STRING_LITERAL>(child)) {
            // Add literal text to result
            result += literal->getText();
        }
        else if (auto expr = dynamic_cast<testlist>(child)) {
            // Evaluate the expression
            auto value = visit(expr);
            // Convert value to string
            result += valueToString(value);
        }
    }
    
    return Value(result);
}
```

### Implementation Steps

1. Add method declaration in `Evalvisitor.h`:
   ```cpp
   std::any visitFormat_string(Python3Parser::Format_stringContext *ctx) override;
   ```

2. Implement in `Evalvisitor.cpp`:
   - Iterate through format_string children
   - Collect FORMAT_STRING_LITERAL tokens as-is
   - Evaluate testlist expressions and convert to string
   - Concatenate all parts into final string
   - Return as Value(string)

3. Implement helper: `std::string valueToString(Value v)`
   - Convert int → string
   - Convert bool → "True" or "False"
   - Convert float → string
   - Convert string → as-is
   - Convert None → "None"
   - Convert list → list representation

4. Handle edge cases:
   - Empty f-strings (if grammar allows)
   - Nested f-strings: `f"{ f"inner" }"`
   - Special characters: `{{` (literal `{`), `}}` (literal `}`)

### Complexity

**Estimated effort**: 8-16 hours
- 2-4 hours: Basic implementation
- 2-4 hours: Type conversion helper
- 2-4 hours: Testing and debugging
- 2-4 hours: Edge cases (nested f-strings, escapes)

## Test Coverage

After fix, should pass:
- test14: Basic f-strings
- test15: Advanced f-strings (nested, complex expressions)

## Priority

**CRITICAL** - Required for OJ submission. Current implementation will fail any test using f-strings.
