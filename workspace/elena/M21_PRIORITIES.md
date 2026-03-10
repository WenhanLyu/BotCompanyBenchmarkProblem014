# M21 Implementation Priorities

**Status:** Post-M20 evaluation complete  
**All Local Tests:** 36/36 passing ✅  
**Last OJ Score:** 25/100 (46/72 tests)

---

## CRITICAL: Three Missing Features

### 1. Default Parameters ⚠️ HIGH PRIORITY
**Why:** Grammar explicitly supports: `tfpdef ('=' test)?`  
**Impact:** Likely blocking 20-30% of AdvancedTests  
**Complexity:** Medium (2-3 hours)

**What to implement:**
```python
def greet(name, greeting="Hello"):
    print(greeting, name)

greet("Alice")           # Should work with default
greet("Bob", "Hi")       # Should work with explicit value
```

**Code changes needed:**
1. `FunctionDef` structure: Add `std::vector<Value> defaultValues`
2. `visitFuncdef`: Parse and evaluate default expressions at definition time
3. `visitTrailer` (function call): Allow fewer args if defaults exist

---

### 2. Keyword Arguments ⚠️ HIGH PRIORITY
**Why:** Grammar explicitly supports: `argument: test | test '=' test`  
**Impact:** Likely blocking 20-30% of AdvancedTests  
**Complexity:** Medium-High (3-4 hours)

**What to implement:**
```python
def describe(name, age, city):
    print(name, age, city)

describe("Alice", 25, "NYC")              # Positional
describe("Bob", age=30, city="LA")        # Mixed
describe(city="SF", name="Carol", age=28) # All keyword (any order)
```

**Code changes needed:**
1. `visitArgument`: Distinguish positional vs keyword arguments
2. Create argument matching algorithm
3. Enforce: positional before keyword
4. Match keywords to parameter names

---

### 3. Chained Comparisons ⚠️ HIGH PRIORITY
**Why:** Grammar requirement + CornerTests 0/10 failure rate  
**Impact:** Likely blocking all 10 CornerTests  
**Complexity:** Medium (2-3 hours)

**What to implement:**
```python
print(1 < 2 < 3)         # True (not: (1 < 2) < 3)

# Critical: evaluate each expression ONCE
def a(): print("a"); return 1
def b(): print("b"); return 2
def c(): print("c"); return 3

result = a() < b() < c()
# Should print: "a", "b", "c" (each once)
# Then: True
```

**Grammar requirement (Section 8.2):**
> "Continuous relational operators like `1<2>3`. The processing method is to split them into adjacent comparisons and connect them with `and`, but **each value is evaluated at most once**"

**Code changes needed:**
1. `visitComparison`: Detect multiple comp_ops
2. Cache intermediate expression values (evaluate once)
3. Chain with short-circuit AND logic

---

## Implementation Order

**Recommended sequence:**
1. **Chained Comparisons** (2-3h) - Most likely to unlock CornerTests
2. **Default Parameters** (2-3h) - Foundation for keyword args
3. **Keyword Arguments** (3-4h) - Builds on default params

**Total estimate:** 7-10 hours for M21

**Expected improvement:** +10-20% OJ score (35-45/100)

---

## Testing Strategy

For each feature:
1. Create local test file (e.g., `test_defaults.py`)
2. Run against Python 3 reference: `python3 test_defaults.py > expected.out`
3. Run against our interpreter: `./code < test_defaults.py > actual.out`
4. Compare: `diff expected.out actual.out`
5. Iterate until identical

---

## Success Criteria

M21 is complete when:
- ✅ All three features implemented
- ✅ All 36 local tests still passing (no regressions)
- ✅ New test files for each feature passing
- ✅ Code committed and pushed to master
- ✅ Ready for next OJ submission

---

## References

- Full evaluation: `workspace/elena/evaluation.md`
- Grammar spec: `docs/grammar.md` (especially Section 8.2)
- Parser grammar: `resources/Python3Parser.g4`
- Implementation: `src/Evalvisitor.{h,cpp}`

---

**Prepared by:** Elena (Code Evaluator)  
**Date:** 2026-03-10
