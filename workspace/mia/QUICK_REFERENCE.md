# Quick Reference - Missing Features

| # | Feature | Status | Test | Impact | Cycles | Priority |
|---|---------|--------|------|--------|--------|----------|
| 1 | **break** | ❌ NOT IMPL | `while: if x==3: break` → doesn't break | 3-5 tests | 1 | **P1** |
| 2 | **continue** | ❌ NOT IMPL | `while: if x==3: continue` → doesn't skip | 3-5 tests | 1 | **P1** |
| 3 | **int()** | ❌ NOT IMPL | `int(3.14)` → returns 0 not 3 | 1-2 tests | 0.5 | **P1** |
| 4 | **float()** | ❌ NOT IMPL | `float(5)` → returns 0 not 5.0 | 1-2 tests | 0.5 | **P1** |
| 5 | **str()** | ❌ NOT IMPL | `str(42)` → returns 0 not "42" | 1-2 tests | 0.5 | **P1** |
| 6 | **bool()** | ❌ NOT IMPL | `bool("")` → wrong result | 1-2 tests | 0.5 | **P1** |
| 7 | **keyword args** | ❌ NOT IMPL | `foo(b=2, a=1)` → None, None | 3-5 tests | 2 | **P2** |
| 8 | **default params** | ❌ NOT IMPL | `def foo(a=5): foo()` → None | 2-3 tests | 1 | **P2** |
| 9 | **subscript []** | ❌ NOT IMPL | `s[0]` → parse error | 2-3 tests | 2 | **P3** |
| 10 | **tuple unpacking** | ✅ WORKS | `a, b = 1, 2` → works! | N/A | N/A | N/A |

## Milestone Grouping

### M10.1: Loop Control (2 cycles, +6-10 tests)
- break statement
- continue statement

### M10.2: Type Conversions (2 cycles, +4-6 tests)  
- int() built-in
- float() built-in
- str() built-in
- bool() built-in

### M10.3: Advanced Functions (3 cycles, +5-8 tests)
- Keyword arguments
- Default parameter values

### M10.4: Edge Cases (2 cycles, +3-5 tests)
- Subscript operator
- None handling
- Empty value edge cases

## Implementation Patterns

| Feature | Pattern | Example Code |
|---------|---------|--------------|
| break/continue | Exception | `throw BreakException();` |
| Type conversion | Function check | `if (funcName == "int") { ... }` |
| Keyword args | Parameter match | `match kwarg names to params` |
| Default params | Value storage | `map<string, Value> defaults` |
| Subscript | Trailer parsing | `if (trailer == "[") { index }` |

## Score Projection

```
Current:       44/75 (58.7%)  25/100 points
After M10.1:   50-54/75       25-30/100
After M10.2:   54-60/75       25-35/100  
After M10.3:   59-68/75       30-50/100
After M10.4:   62-73/75       35-65/100

Target:        65-70/75 (87-93%)  ~50-65/100 points
```

## Risk Assessment

| Milestone | Risk | Reason |
|-----------|------|--------|
| M10.1 | **LOW** | Exception pattern proven (ReturnException) |
| M10.2 | **LOW** | Well-specified in grammar section 11 |
| M10.3 | **MEDIUM** | Complex parameter matching logic |
| M10.4 | **MEDIUM** | Negative indexing, bounds checking |

## Next Action

👉 **Recommend implementing M10.1 + M10.2 first** (4 cycles, +10-16 tests, LOW risk)
