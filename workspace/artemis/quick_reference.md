# Quick Reference: Implementation Complexity

**Analyst:** Artemis | **Date:** 2026-03-02

---

## TL;DR

| Feature | Complexity | Cycles (B/R/W) | Risk | Implement First? |
|---------|-----------|----------------|------|------------------|
| **Return Statements** | MEDIUM | 2-3 / 3-4 / 5-6 | LOW-MEDIUM | ✅ YES |
| **F-Strings** | HIGH | 4-6 / 6-8 / 10-12 | MEDIUM-HIGH | ⏸️ SECOND |

**Total Project Estimate:** 14 cycles (12 cycles + 2 buffer)

---

## Return Statements (test13)

### What It Needs
- Exception-based control flow (throw/catch)
- Return value capture and propagation
- Proper scope restoration on function exit

### Implementation Size
- **~40 LOC** across 3 locations in 2 files
- ReturnException struct: ~10 LOC
- visitReturn_stmt method: ~15 LOC  
- Function call modification: ~15 LOC

### Why It's MEDIUM Complexity
✅ **Pros:**
- Well-defined pattern (exception-based control flow)
- Small code footprint
- Liam's test infrastructure ready
- No dependencies

⚠️ **Cons:**
- Exception pattern not yet established in codebase
- Need careful scope restoration
- Recursive calls need testing

### Effort Breakdown
- **Best case (2-3 cycles):** Clean implementation, tests pass immediately
- **Realistic (3-4 cycles):** Works but needs debugging, some edge cases
- **Worst case (5-6 cycles):** Scope issues, multiple debugging iterations

---

## F-Strings (test14-15)

### What It Needs
- Parse format_string grammar rule
- Iterate FORMAT_STRING_LITERAL and testlist children
- Evaluate expressions in `{}` braces
- Convert all Value types to strings
- Handle nested f-strings
- Handle escape sequences (`{{` → `{`, `}}` → `}`)

### Implementation Size  
- **~150-200 LOC** across 3 locations in 2 files
- visitFormat_string method: ~120-150 LOC
- visitAtom modification: ~10 LOC
- Header declaration: ~2 LOC
- Optional valueToString helper: ~30 LOC (reuse from print)

### Why It's HIGH Complexity
✅ **Pros:**
- Grammar already supports it
- Can reuse print's value-to-string logic
- Recursive evaluation handles nesting automatically

⚠️ **Cons:**
- Lexer format_mode state management (unknown behavior)
- Complex children iteration logic
- Must handle all Value types correctly
- Nested f-strings might break lexer
- Edge cases: empty strings, escape sequences, tuples

### Effort Breakdown
- **Best case (4-6 cycles):** Parser works cleanly, minimal debugging
- **Realistic (6-8 cycles):** Lexer debugging, formatting tweaks, nested f-string fix
- **Worst case (10-12 cycles):** Lexer broken, parser modifications, tuple support needed

---

## Risk Comparison

### Return Statements
| Risk | Probability | Impact | Mitigation |
|------|------------|--------|------------|
| Exception conflicts | Low | High | Use distinct type |
| Scope restoration | Medium | Medium | Test nested functions |
| test13 edge cases | Low | Medium | Use Liam's tests |
| Regression | Low | Low | Run test0-12 |

**Overall: LOW-MEDIUM RISK**

### F-Strings
| Risk | Probability | Impact | Mitigation |
|------|------------|--------|------------|
| Lexer broken | Medium | High | Incremental testing |
| Nested f-strings fail | Medium | High | Test early |
| Tuple support needed | Low | Very High | Avoid if possible |
| Formatting mismatch | Medium | Low | Reuse print logic |

**Overall: MEDIUM-HIGH RISK**

---

## Implementation Order

```
Step 1: M6 - Return Statements (4 cycles)
  ├─ Cycle 1: Implement ReturnException + visitReturn_stmt
  ├─ Cycle 2: Modify function calls, test simple returns  
  ├─ Cycle 3: Test recursive returns, debug edge cases
  └─ Cycle 4: Validate test13, fix regressions
       ↓
       ✅ test13 PASSES
       ↓
Step 2: M7 - F-Strings (8 cycles)
  ├─ Cycle 1-2: Implement visitFormat_string structure
  ├─ Cycle 3: Value-to-string conversion
  ├─ Cycle 4-5: Test all value types and expressions
  ├─ Cycle 6: Nested f-strings
  ├─ Cycle 7: Escape sequences and edge cases
  └─ Cycle 8: Validate test14-15, regression testing
       ↓
       ✅ test14, test15 PASS
       ↓
Step 3: Final Validation (2 cycles buffer)
  └─ OJ submission preparation
       ↓
       🎯 PROJECT COMPLETE
```

---

## Code Snippets

### Return Implementation (Core Logic)

```cpp
// In Evalvisitor.h
struct ReturnException : public std::exception {
    Value returnValue;
    explicit ReturnException(const Value& val) : returnValue(val) {}
};

// In Evalvisitor.cpp - visitReturn_stmt
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
    Value returnValue = ctx->testlist() 
        ? evaluateTestlist(ctx->testlist()) 
        : Value(std::monostate{});
    throw ReturnException(returnValue);
}

// In Evalvisitor.cpp - function call modification
Value returnValue = Value(std::monostate{});
try {
    visit(funcDef.body);
} catch (const ReturnException& ret) {
    returnValue = ret.returnValue;
}
variables = savedVariables;
return returnValue;
```

### F-String Implementation (Core Logic)

```cpp
// In Evalvisitor.cpp - visitFormat_string
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
    std::string result;
    
    auto literals = ctx->FORMAT_STRING_LITERAL();
    auto expressions = ctx->testlist();
    
    // Interleave literals and expression results
    for (size_t i = 0; i < std::max(literals.size(), expressions.size()); i++) {
        if (i < literals.size()) {
            result += processEscapes(literals[i]->getText());
        }
        if (i < expressions.size()) {
            Value val = evaluate(expressions[i]);
            result += valueToString(val);  // Reuse from print logic
        }
    }
    
    return Value(result);
}
```

---

## Testing Strategy

### Returns Testing (Progressive)
1. ✅ Simple: `return 42`
2. ✅ Early: `if x: return y`
3. ✅ Recursive: `return gcd(y, x % y)`
4. ✅ test13: Full Pollard Rho

### F-Strings Testing (Progressive)
1. ✅ Static: `f"hello"`
2. ✅ Simple expr: `f"{x}"`
3. ✅ All types: `f"{int} {bool} {str} {float}"`
4. ✅ Complex: `f"{114514 + 666666 - 1919810}"`
5. ✅ String ops: `f"{"a" + " " + "b"}"`
6. ✅ Nested: `f"{ f"x" }"`
7. ✅ Escapes: `f"{{not interpolated}}"`
8. ✅ test14, test15: Full validation

---

## Critical Paths

### Return Statements Critical Path
```
Implement ReturnException → Implement visitReturn_stmt → 
Modify function calls → Test simple returns → 
Test recursive returns → Validate test13 → DONE
```

**Estimated: 3-4 cycles (realistic)**

### F-Strings Critical Path
```
Implement visitFormat_string → Handle literals → 
Handle expressions → Value-to-string conversion → 
Test all types → Nested f-strings → 
Escape sequences → Validate test14-15 → DONE
```

**Estimated: 6-8 cycles (realistic)**

---

## Recommendations

1. ✅ **Implement returns first** - Lower risk, clearer path
2. ✅ **Allocate 4 cycles for returns** - Includes buffer
3. ✅ **Allocate 8 cycles for f-strings** - Includes debugging
4. ✅ **Extract valueToString helper** - Code reuse opportunity
5. ⚠️ **Test incrementally** - Both features need progressive validation
6. ⚠️ **Watch for regressions** - Run test0-12 after each major change

---

## Unknown Risks

1. **BigInteger actual status?** - M5.1 supposedly complete but test report shows failures
2. **Lexer format_mode behavior?** - Might need grammar debugging for f-strings
3. **Break/continue needed?** - Not in basic tests, but might appear in advanced tests
4. **Tuple support?** - F-strings can have tuples, but test14-15 don't use them

---

## Success Criteria

### M6 (Returns) Success
- ✅ All Liam's return tests pass
- ✅ test13 passes (47 lines match expected output)
- ✅ test0-12 still pass (no regressions)
- ✅ No memory leaks (valgrind clean)

### M7 (F-Strings) Success  
- ✅ test14 passes (7 lines match expected output)
- ✅ test15 passes (3 lines match expected output)
- ✅ All value types stringify correctly
- ✅ Nested f-strings work
- ✅ test0-13 still pass (no regressions)

### Project Complete
- ✅ test0-15 all pass (16/16 basic tests)
- ✅ BigInteger tests pass (20/20 if M5.1 complete)
- ✅ Ready for OJ submission

---

**For detailed analysis, see `implementation_complexity_evaluation.md`**
