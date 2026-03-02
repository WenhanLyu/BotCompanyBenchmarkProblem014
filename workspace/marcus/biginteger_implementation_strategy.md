# BigInteger Implementation Strategy: When and How?

## The Question
Should we implement BigInteger early (complex, high-value) or build basic features first (simpler, establishes foundation)?

## TL;DR Recommendation: **Hybrid Approach**

**Phase 1 (1-2 cycles):** Minimal viable framework → get test1 working
**Phase 2 (3-8 cycles):** Complete BigInteger → pass all 20 BigInteger tests  
**Phase 3+ (9-22 cycles):** Other features incrementally

## Analysis

### BigInteger Early: Arguments

**PRO:**
1. **30% of total score** (20/66 test cases) - highest single component value
2. **Most complex component** - better to tackle when team is fresh and focused
3. **Can develop independently** - doesn't depend on control flow, functions, etc.
4. **Clear success metric** - 20 BigInteger tests provide immediate validation
5. **De-risks project** - if we can't get BigInteger working, we know early
6. **Psychological boost** - solving hardest problem builds confidence

**CON:**
1. **Delays visible progress** - no working interpreter for several cycles
2. **Can't fully test integration** until visitor framework exists
3. **Might discover design issues** that require BigInteger redesign
4. **Time pressure** - if BigInteger takes too long, other features suffer

### Basic Features First: Arguments

**PRO:**
1. **Quick wins** - test1 (print "Hello World") can work in 1-2 cycles
2. **Establishes foundation** - Value system, visitor pattern in place
3. **Validates architecture** - ensures design works before committing
4. **Momentum** - seeing green tests motivates team
5. **Integration testing** - can test BigInteger with real visitor framework

**CON:**
1. **Defers hardest problem** - might run out of time later
2. **Two-pass overhead** - need to implement int operations twice (native, then BigInteger)
3. **Risk accumulation** - if BigInteger fails late, whole project at risk
4. **Loss of focus** - easier to get distracted by "just one more feature"

## Decision: Hybrid Approach

### Phase 1: Minimal Foundation (1-2 cycles)
**Goal:** Prove the architecture works

**Implement:**
- Value type with basic types only (bool, int64_t, double, string)
- Simple print function
- String literals
- Basic visitor structure
- Variable assignment (simple)

**Test Target:** test1 (print "Hello, World!")

**Why this first:**
- Validates ANTLR integration
- Proves std::variant approach
- Establishes development workflow
- Takes only 1-2 cycles
- Provides foundation for testing BigInteger

**Code estimation:** ~200-300 LOC

### Phase 2: Complete BigInteger (3-8 cycles)
**Goal:** Solve hardest problem while fresh

**Implement:**
- Full BigInteger class (500-800 LOC)
- All arithmetic: +, -, *, /, //, %
- Comparison operators: <, <=, >, >=, ==, !=
- Python semantics (floor division, modulo)
- String conversion (I/O)
- Integration with Value system

**Test Target:** BigIntegerTest0-19 (all 20 bigint tests)

**Why this second:**
- Foundation exists to test it
- Team is still fresh
- 30% of score secured early
- De-risks entire project
- Clear success metric

**Code estimation:** 500-800 LOC

### Phase 3+: Incremental Features (9-22 cycles)
**Goal:** Complete remaining features

**Implement in order:**
- Control flow (if/while/break/continue) → test3-7
- Functions (def, call, return) → test8-11
- Advanced functions (defaults, keywords) → test12-13
- F-strings and tuples → test14-15
- Polish and edge cases

**Why this order:**
- Each phase builds on previous
- Test-driven (target specific tests)
- Can stop at any milestone
- Incremental risk

## Implementation Plan for BigInteger

### Data Structure
```cpp
class BigInteger {
private:
    std::vector<int> digits;  // Base 10^9, LSB first
    bool negative;
    static const int BASE = 1000000000;
};
```

**Why base 10^9:**
- Fits in uint32_t (max 4,294,967,295)
- Efficient: 9 decimal digits per int
- Reasonable string I/O conversion
- Standard competitive programming approach

### Algorithm Complexity

| Operation | Algorithm | Complexity | Fast Enough? |
|-----------|-----------|------------|--------------|
| Addition | Grade school | O(n) | ✅ Yes |
| Subtraction | Grade school | O(n) | ✅ Yes |
| Multiplication | Grade school | O(n²) | ✅ Yes (25M ops for 5000 digits) |
| Division | Binary search quotient | O(n²) | ✅ Yes |
| Comparison | Lexicographic | O(n) | ✅ Yes |

**Note:** No need for Karatsuba or FFT multiplication. Simple O(n²) is fast enough.

### Critical Implementation Details

**1. Floor Division (Python semantics)**
```
C++:    -5 / 3 = -1   (truncate toward zero)
Python: -5 // 3 = -2  (floor toward -infinity)
```
Formula: `floor(a/b)` not `truncate(a/b)`

**2. Modulo (follows divisor sign)**
```
a % b = a - (a // b) * b
```
Result has same sign as divisor, not dividend!

**3. Zero normalization**
- Ensure zero is always positive (not negative zero)
- Remove leading zeros after operations

**4. Sign handling**
```
pos + pos → pos
pos + neg → subtract magnitudes, take sign of larger
neg + neg → neg
pos - neg → add magnitudes, positive
neg - pos → add magnitudes, negative
```

### Testing Strategy

**Unit tests (manual validation):**
```cpp
// Small numbers - verify logic
assert(BigInteger("5") + BigInteger("3") == BigInteger("8"));
assert(BigInteger("-5") + BigInteger("3") == BigInteger("-2"));

// Floor division
assert(BigInteger("-5") / BigInteger("3") == BigInteger("-2"));  // NOT -1!

// Modulo
assert(BigInteger("-5") % BigInteger("3") == BigInteger("1"));   // NOT -2!

// Large numbers
BigInteger large("123456789012345678901234567890");
// ... operations
```

**Integration tests:**
Run against BigIntegerTest0-19, compare with Python output.

### Risk Mitigation

| Risk | Probability | Mitigation |
|------|-------------|------------|
| Floor division wrong | High | Test extensively with negative numbers |
| Modulo sign wrong | High | Verify against Python: a % b = a - (a // b) * b |
| Performance too slow | Low | Profile, optimize hot paths if needed |
| Integer overflow | Medium | Use int64_t for intermediate calculations |
| Leading zeros bug | Medium | Normalize after every operation |

## Time Budget Estimate

| Phase | Cycles | Outcome |
|-------|--------|---------|
| Phase 1: Foundation | 1-2 | test1 passing |
| Phase 2: BigInteger | 5-6 | BigIntegerTest0-19 passing (30% score) |
| Phase 3: Control flow | 3-4 | test3-7 passing |
| Phase 4: Functions | 4-5 | test8-13 passing |
| Phase 5: Advanced | 3-4 | test14-15 passing |
| **Total** | **16-21** | All 36 local tests passing |

## Why This Approach Wins

1. **Quick validation** - Foundation works in 1-2 cycles
2. **Early de-risking** - Hardest component (30% score) done by cycle 8
3. **Psychological momentum** - Test1 works quickly, BigInteger victory early
4. **Testing quality** - Can validate BigInteger against real framework
5. **Flexibility** - Can adjust timeline based on BigInteger complexity
6. **Clear milestones** - Each phase has concrete deliverable

## Fallback Plan

**If BigInteger takes too long (>10 cycles):**
1. Get basic operations working (+, -, *, print)
2. Move to other features
3. Return to division/modulo later
4. Better to get 70% score (other tests) than 0% (stuck on BigInteger)

**If stuck on floor division semantics:**
1. Test extensively with Python REPL
2. Study Python source code (longobject.c)
3. Ask for help / create issue
4. Don't spin - move forward

## Conclusion

**Hybrid approach balances:**
- ✅ Risk management (tackle BigInteger early but not first)
- ✅ Velocity (quick win with test1)
- ✅ Quality (foundation to test BigInteger properly)
- ✅ Psychology (momentum from early success, confidence from hard problem)

**Start with foundation (1-2 cycles), then commit to BigInteger (5-6 cycles). This is the path.**
