# F-String Implementation Roadmap

## Executive Summary

**Complexity Assessment:** MEDIUM  
**Estimated LOC:** 150-250 lines  
**Estimated Cycles:** 3-4 cycles  
**Risk Level:** MEDIUM (lexer complexity, nested evaluation)  
**Dependencies:** None (current architecture sufficient)  
**Recommended Priority:** After BigInteger (M5) is complete

## Test Case Analysis

### Test14: Basic F-Strings
- Integer interpolation: `f"text { 114514 } text"`
- Boolean interpolation: `f"text { True } text"`
- String interpolation: `f"text { "Hello" } text"`
- Float interpolation: `f"text { 1.0 } text"`
- Expression evaluation: `f"text { 114514 + 666666 - 1919810 } text"`
- No formatting needed (plain text)
- Entire f-string as expression: `f"{ "text" }"`

### Test15: Complex F-Strings
- Multiple interpolations: `f"{ expr1 } text { expr2 }"`
- String concatenation inside: `f"{ "a" + " " + "b" }"`
- Complex arithmetic: `f"{ (3 * 4) * (6 * 16) + 1919810 // 114514 }"`
- **Nested f-strings**: `f"{ f"inner" } outer"`

**Key Requirements:**
1. Parse f-string tokens (FORMAT_QUOTATION, FORMAT_STRING_LITERAL, QUOTATION)
2. Evaluate arbitrary Python expressions inside `{}`
3. Convert expression results to strings
4. Handle nested f-strings recursively
5. Concatenate literal text and evaluated expressions

## Architecture Analysis

### ANTLR Grammar Support

The Python3Lexer already supports f-strings through a **mode-based lexing** system:

```
// Lexer state variables
int format_mode = 0;      // Depth of nested f-strings
bool expr_mode = false;   // Whether we're inside {}

// Token sequence for f"text { expr } text"
FORMAT_QUOTATION   : 'f"' { ++format_mode; expr_mode = false; }
FORMAT_STRING_LITERAL : {format_mode > 0 && !expr_mode}? (chars | '{{' | '}}')+ 
OPEN_BRACE        : '{' { opened++; expr_mode = true; }
// ... expression tokens ...
CLOSE_BRACE       : '}' { opened--; expr_mode = false; }
FORMAT_STRING_LITERAL : more literal text
QUOTATION         : {format_mode > 0}? '"' { --format_mode; }
```

**Parser rule:**
```
format_string: FORMAT_QUOTATION (FORMAT_STRING_LITERAL | '{' testlist '}')* QUOTATION;
```

This means the parser **already tokenizes and parses f-strings**. The lexer handles:
- Switching modes when it sees `f"`
- Recognizing literal text vs expressions
- Tracking nesting depth
- Properly pairing `{` and `}`

### Current Implementation Status

**Not implemented:**
- `visitFormat_string()` - No implementation exists

**Available infrastructure:**
- `visitTest()` - Can evaluate any Python expression ✅
- `visitTestlist()` - Can handle multiple expressions ✅
- Value → string conversion (via print logic) ✅
- String concatenation (via + operator) ✅

## Implementation Strategy

### Approach: Direct String Building

**Algorithm:**
```cpp
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
    std::stringstream result;
    
    // Skip FORMAT_QUOTATION (child 0) and QUOTATION (last child)
    for (size_t i = 1; i < ctx->children.size() - 1; i++) {
        auto child = ctx->children[i];
        
        if (auto literal = dynamic_cast<antlr4::tree::TerminalNode*>(child)) {
            // FORMAT_STRING_LITERAL - append as-is (handle {{ and }})
            std::string text = literal->getText();
            text = replace(text, "{{", "{");
            text = replace(text, "}}", "}");
            result << text;
        }
        else {
            // Expression node: '{ testlist }'
            // Evaluate the testlist and convert to string
            Value val = std::any_cast<Value>(visit(child->children[1])); // testlist
            result << valueToString(val);
        }
    }
    
    return Value(result.str());
}
```

**Key Functions Needed:**

1. **valueToString(Value val) → string**
   - Convert any Value type to its string representation
   - Handle int, bool, float, string, None
   - Use same logic as print() function
   - ~30 LOC

2. **replaceAll(string& str, string from, string to)**
   - Replace escape sequences {{ and }}
   - ~10 LOC

### Implementation Details

#### 1. Literal Text Handling
- FORMAT_STRING_LITERAL tokens contain literal text
- Must unescape `{{` → `{` and `}}` → `}`
- Plain text outside expressions

#### 2. Expression Evaluation
- Each `{ testlist }` is a subtree
- Use existing `visitTest()` or `visitTestlist()` to evaluate
- Returns a Value
- Convert Value to string using `valueToString()`

#### 3. Nested F-Strings
- The lexer handles nesting via `format_mode` counter
- When evaluating `{ testlist }`, the testlist might contain another `format_string`
- **Recursion works automatically** - visitFormat_string calls visit() which may call visitFormat_string again

#### 4. Type Conversion Rules
```cpp
std::string valueToString(const Value& val) {
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return "None";
        } else if constexpr (std::is_same_v<T, bool>) {
            return arg ? "True" : "False";
        } else if constexpr (std::is_same_v<T, int>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, double>) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << arg;
            return ss.str();
        } else if constexpr (std::is_same_v<T, std::string>) {
            return arg;
        }
    }, val);
}
```

**Note:** Float formatting must match test expectations. Test14 expects `1.0` (not `1.000000`).

#### 5. Error Handling
- Invalid expressions inside `{}` → runtime error (let existing error handling propagate)
- Empty braces `{}` → likely parse error (grammar may not allow)
- Unclosed braces → lexer/parser error (already handled)

## Integration with Current Architecture

### No Breaking Changes Required
- Value type already supports string ✅
- Expression evaluation already works ✅
- String operations already work ✅
- visitAtom() already handles STRING tokens ✅

### New Code Required
1. **visitFormat_string()** implementation (~80 LOC)
2. **valueToString()** helper function (~40 LOC)
3. **replaceAll()** helper function (~10 LOC)
4. Update **visitAtom()** to handle format_string alternative (~5 LOC)

**Total new code:** ~135 LOC

### Code Locations
- **Evalvisitor.h**: Add `visitFormat_string()` declaration, `valueToString()` helper
- **Evalvisitor.cpp**: Implement both functions

## Testing Strategy

### Phase 1: Basic Interpolation (test14 subset)
```python
print(f"text { 114514 } text")
print(f"text { True } text")
print(f"text { 1.0 } text")
```

### Phase 2: Expressions (test14 complete)
```python
print(f"{ 114514 + 666666 - 1919810 }")
print(f"Formatted string without formatting is correct.")
```

### Phase 3: Complex Cases (test15)
```python
print(f"{ "a" + " " + "b" }")
print(f"{ (3 * 4) * (6 * 16) + 1919810 // 114514 }")
```

### Phase 4: Nested F-Strings (test15)
```python
print(f"{ f"inner" } outer")
```

### Regression Testing
- Ensure test0-13 still pass
- No changes to existing visitor methods (except visitAtom)

## Risks and Mitigation

### Risk 1: Float Formatting Inconsistencies
**Problem:** Different precision expectations (1.0 vs 1.000000)  
**Mitigation:** 
- Analyze test output carefully
- Use std::setprecision(1) for simple cases
- May need context-aware formatting

**Likelihood:** MEDIUM  
**Impact:** LOW (easy to fix with trial and error)

### Risk 2: Nested F-String Recursion
**Problem:** Deep nesting could cause stack overflow  
**Mitigation:**
- Python itself limits nesting depth
- Tests unlikely to have pathological cases
- Recursion is natural solution, matches language semantics

**Likelihood:** LOW  
**Impact:** LOW

### Risk 3: Escape Sequence Handling
**Problem:** {{ and }} must become { and } in output  
**Mitigation:**
- Simple string replacement
- Test with explicit cases

**Likelihood:** LOW  
**Impact:** LOW (trivial fix)

### Risk 4: Context Rule Complexity
**Problem:** Format_stringContext structure might be different than expected  
**Mitigation:**
- Print AST structure during development
- Use ctx->toStringTree() for debugging
- Examine actual parse tree with simple test

**Likelihood:** MEDIUM  
**Impact:** MEDIUM (requires AST inspection, 1-2 hour debug time)

## Comparison with Other Features

| Feature | LOC | Cycles | Complexity | Dependencies |
|---------|-----|--------|------------|--------------|
| M3.1 Comparison | ~50 | 2 | LOW | None |
| M4.1 Augmented Assignment | ~150 | 2 | MEDIUM | While loops, logical ops |
| M4.2 String Concat | ~30 | 2 | LOW | None |
| M4.3 Function Params | ~80 | 2 | MEDIUM | Function def |
| **M4.4 F-Strings** | **~150** | **3-4** | **MEDIUM** | **None** |
| M5 BigInteger | ~850 | 8-12 | HIGH | Major refactor |

**Observation:** F-strings are comparable in scope to M4.1 (augmented assignment) but **simpler** because:
- No control flow changes
- No new operators
- Just string building + expression evaluation
- Lexer/parser already handle tokenization

## Recommended Implementation Plan

### Cycle 1: Setup and Basic Cases (1 cycle)
**Goal:** Get simple f-string working

1. Add `visitFormat_string()` stub that returns empty string
2. Implement `valueToString()` helper
3. Implement basic literal text concatenation
4. Test: `f"plain text"` → "plain text"
5. Test: `f"{ 42 }"` → "42"

**Acceptance:** Basic integer interpolation works

### Cycle 2: Expression Evaluation (1 cycle)
**Goal:** Complete test14

1. Implement full expression evaluation in `{}`
2. Handle all Value types (int, bool, float, string, None)
3. Handle escape sequences ({{ and }})
4. Test all test14 cases

**Acceptance:** test14 passes completely

### Cycle 3: Complex Cases and Nesting (1-2 cycles)
**Goal:** Complete test15

1. Verify multiple interpolations work
2. Test string concatenation inside expressions
3. Test complex arithmetic
4. **Test nested f-strings** (most complex case)
5. Run full regression suite

**Acceptance:** test15 passes, no regressions on test0-13

### Optional Cycle 4: Polish (if needed)
**Goal:** Handle edge cases

1. Float formatting edge cases
2. Performance optimization (if needed)
3. Error message improvements
4. Documentation

**Acceptance:** All tests pass, code is clean

## Code Quality Considerations

### Maintainability
- Small, focused function (visitFormat_string)
- Clear separation: text vs expressions
- Reusable valueToString() helper (may be useful elsewhere)

### Performance
- Linear pass through format_string children - O(n)
- String concatenation via stringstream - efficient
- No significant performance concerns

### Testability
- valueToString() can be unit tested independently
- Each phase has clear test cases
- Easy to isolate failures (literal text vs expression evaluation)

## Alternative Approaches Considered

### Alternative 1: Token-by-Token Processing
**Approach:** Manually iterate through tokens instead of AST nodes  
**Pros:** More control over lexer output  
**Cons:** Bypasses parser, harder to handle nesting, not idiomatic ANTLR  
**Verdict:** ❌ Not recommended

### Alternative 2: String Replacement
**Approach:** Get full f-string text, find `{}`, replace with eval results  
**Pros:** Simple conceptually  
**Cons:** Fragile (what about nested braces?), ignores AST, hard to handle nesting  
**Verdict:** ❌ Not recommended

### Alternative 3: Visitor Pattern (Recommended) ✅
**Approach:** Use ANTLR visitor to walk format_string AST  
**Pros:** Natural, handles nesting, leverages parser, idiomatic  
**Cons:** Need to understand Format_stringContext structure  
**Verdict:** ✅ **RECOMMENDED** - matches project architecture

## Dependencies and Sequencing

### Before F-Strings (Prerequisites)
- ✅ Basic types (int, bool, string, float, None) - M2
- ✅ Expression evaluation (arithmetic, comparison) - M3, M3.1
- ✅ String operations (+) - M4.2
- ✅ Function calls (for nested f-strings) - M4.3

### After F-Strings (Blockers)
- None - F-strings don't block any other features

### Recommended Position in Roadmap
**Current:** M4.4 (after function params)  
**Rationale:**
- All prerequisites are met
- Independent feature (doesn't block others)
- Medium complexity (good momentum builder after BigInteger)
- test13 is more critical (requires BigInteger + return + global)

**Alternative:** Could be M4.4 or even delayed to M6, but makes sense to do before tackling test13 complexity.

## BigInteger Interaction

**Question:** Do f-strings need BigInteger support?

**Analysis:**
- test14, test15 don't use large numbers
- F-strings just call valueToString()
- If BigInteger is added to Value variant, valueToString() needs BigInteger case

**Recommendation:**
1. **If implementing BEFORE BigInteger (M5):**
   - Implement for existing types only
   - ~150 LOC

2. **If implementing AFTER BigInteger (M5):**
   - Add BigInteger case to valueToString()
   - ~180 LOC (add 30 LOC for BigInteger formatting)

**Both approaches work.** Slight preference for AFTER M5 to avoid touching code twice.

## Conclusion

### Complexity: MEDIUM ✅
- Well-defined problem
- Lexer/parser handle hard parts
- Clear algorithm
- Moderate LOC (~150)

### Feasibility: HIGH ✅
- No architectural changes needed
- No dependencies on incomplete features
- Clear test cases
- Established pattern (visitor)

### Effort: 3-4 cycles ✅
- Comparable to M4.1 and M4.3
- Room for iteration and testing
- Low risk of overrun

### Recommendation: Implement after M5 (BigInteger) ✅
**Rationale:**
1. BigInteger is 30% of OJ score (higher priority)
2. test13 requires BigInteger anyway
3. Better to have BigInteger in Value before implementing valueToString()
4. F-strings don't block other features
5. Clear path forward after M5 complete

### Implementation Sequence
```
M5: BigInteger (in progress)
  ↓
M5.1: Fix division bugs
  ↓
M5.2: Verify all 20 BigInteger tests
  ↓
M4.4: F-Strings (test14, test15) ← 3-4 cycles
  ↓
M6: Return + Global (test13) ← requires BigInteger + new features
  ↓
M7: Final polish, OJ submission
```

---

## Appendix: Quick Reference

### Key Files to Modify
- `src/Evalvisitor.h` - Add visitFormat_string() declaration
- `src/Evalvisitor.cpp` - Implement visitFormat_string() and valueToString()

### Key Functions
```cpp
// Main implementation
std::any visitFormat_string(Python3Parser::Format_stringContext *ctx);

// Helper for type conversion
std::string valueToString(const Value& val);

// Helper for escape sequences
std::string replaceAll(std::string str, const std::string& from, const std::string& to);
```

### Test Commands
```bash
# Test f-strings
./code < testcases/basic-testcases/test14.in
./code < testcases/basic-testcases/test15.in

# Regression test
./test_m1.sh  # Or whatever test script exists
```

### Debugging Tips
1. Print AST structure: `ctx->toStringTree(parser)`
2. Print children count: `ctx->children.size()`
3. Print child types: `typeid(*child).name()`
4. Test valueToString() independently with known Values

---

**Document Version:** 1.0  
**Author:** Marcus (Architecture Researcher)  
**Date:** 2026-03-02  
**Status:** Ready for implementation after M5 complete
