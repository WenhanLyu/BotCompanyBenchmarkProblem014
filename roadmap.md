# Project Roadmap: Python Interpreter (ACMOJ 2515)

## Goal
Implement a C++ Python interpreter using ANTLR4 visitor pattern that passes all 66 OJ test cases.

## Test Point Distribution
- BigIntegerTest: 1-20 (big integer arithmetic)
- Sample: 21-34 (basic Python features)
- AdvancedTest: 35-52 (functions, scope, complex control flow)
- ComplexTest: 53-56 (complex programs)
- CornerTest: 57-66 (edge cases)

## Milestones

### M1: Core Python Interpreter Implementation (cycles: 8)
**Status: IN PROGRESS**

Implement the complete EvalVisitor in `src/Evalvisitor.h` and `src/Evalvisitor.cpp` covering:
1. **Big integer support** - Python's arbitrary precision integers using C++ (boost or custom)
2. **Basic values**: int, float, bool, str, None, tuple
3. **Expressions**: arithmetic, comparison, logical, chained comparisons
4. **Statements**: assignment, augmented assignment, multi-assignment, if/elif/else, while, break, continue
5. **Functions**: def, return, recursion, default params, keyword args, positional args
6. **Built-ins**: print, int(), float(), str(), bool()
7. **F-strings**: formatted string literals with expression evaluation
8. **Scope**: global scope accessible in functions without `global` keyword
9. **Operator semantics**: floor division, modulo with Python semantics, string repetition/concatenation

Acceptance criteria:
- All local basic-testcases pass
- All local bigint-testcases pass
- CMake build succeeds

### M2: Fix & Polish (cycles: 4) - if needed
Address any issues from OJ feedback.

## Lessons Learned
- (to be filled)

## Cycle History
- Cycle 1 (Athena): Project started, analyzed requirements, defined M1
