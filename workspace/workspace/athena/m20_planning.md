# M20 Planning - Post M19 Analysis

## Current State

**Completed Milestones:**
- M1-M16 ✅
- M18 ✅ (Investigation - found critical bugs)
- M19 ✅ (Critical bug fixes - string mult + multiple returns)

**Blocked:**
- M17 (OJ #5 results analysis) - still waiting for external data

**Repository State:**
- All code builds cleanly
- 35/35 available local tests passing (100%)
- M19 features verified working
- Latest commits: 0e5f2ff, eaa6060

## Features Implemented

### Core Language Features ✅
- Variables, assignment, augmented assignment
- Types: int, float, bool, str, None, BigInteger, Tuple
- Operators: arithmetic, comparison, logical
- Control flow: if/elif/else, while, break, continue
- Functions: def, parameters, return (single + multiple)
- Built-ins: print(), int(), float(), str(), bool()
- F-strings
- String operations: concatenation, multiplication

### Known Missing Features (from M18/roadmap)

**From roadmap M13 analysis:**
1. **Default Parameters** - Functions with default argument values (2 cycles estimated)
2. **Keyword Arguments** - Named parameters in function calls (2 cycles estimated)
3. **List Operations** - Indexing, slicing, methods (2-3 cycles estimated)
4. **For Loops** - for...in iteration (3 cycles estimated)
5. **Advanced subscripting** - list/tuple/string indexing (2 cycles estimated)

**From Elena's M18 analysis (issue #147):**
- Default parameters missing
- Keyword arguments missing
- Note: Multiple return values was listed but NOW FIXED in M19

## Strategic Options

### Option A: Continue Without OJ #5 Data (Investigation Approach)
Similar to M18, investigate independently:
- Analyze grammar for mandatory features
- Scan test files for feature usage
- Prioritize based on grammar requirements + test evidence
- Risk: May implement features that don't impact OJ score much

### Option B: Wait for OJ #5 Data
- Pro: Empirical data drives decisions (proven successful in M9.1, M13, M15)
- Con: Unknown wait time (already 4+ hours since tag push)
- Con: Wastes cycle if data doesn't arrive

### Option C: Implement High-Confidence Features
Choose features that:
- Are grammar-mandated (objective requirement)
- Appear in local test files (evidence-based)
- Have clear implementation path (low risk)

## Recommendation: Option C

**Rationale:**
1. M18 showed we can make progress without OJ data
2. M19 was successful based on M18 investigation
3. Some features are objectively needed (grammar requirements)
4. Can pivot if OJ #5 arrives during work

**High-Confidence Feature Candidates:**

### 1. List/Tuple Subscripting (HIGHEST PRIORITY)
**Evidence:**
- Grammar requires subscript operations
- Fundamental Python feature
- Likely used in many OJ tests

**Scope:**
- Indexing: `a[0]`, `s[2]`
- Negative indexing: `a[-1]`
- Works on lists, tuples, strings

**Estimated Effort:** 2 cycles
**Expected Impact:** +3-5 tests (conservative)

### 2. For Loops
**Evidence:**
- Grammar includes for statement
- Fundamental control flow
- Required for many algorithms

**Scope:**
- Basic for...in loops
- range() function
- Iteration over lists/tuples/strings

**Estimated Effort:** 3 cycles
**Expected Impact:** +4-6 tests (conservative)

### 3. Default Parameters
**Evidence:**
- Elena identified as missing in M18
- Common Python feature
- Grammar supports it

**Scope:**
- Function definitions with defaults: `def f(a, b=10):`
- Calling with/without optional args

**Estimated Effort:** 2 cycles
**Expected Impact:** +2-4 tests (conservative)

## M20 Recommendation

**Title:** List/Tuple/String Subscripting

**Rationale:**
- Most fundamental missing feature
- Grammar-mandated (objective need)
- Enables other features (for loops need iteration)
- Clear implementation path
- Low risk (well-understood semantics)

**Deliverables:**
1. Positive indexing: `a[i]` for lists, tuples, strings
2. Negative indexing: `a[-1]` for last element
3. Bounds checking with appropriate errors
4. Integration with existing Value type system

**Acceptance Criteria:**
- [ ] Can index lists: `[1,2,3][0]` returns 1
- [ ] Can index tuples: `(10,20,30)[1]` returns 20
- [ ] Can index strings: `"hello"[1]` returns "e"
- [ ] Negative indexing works: `[1,2,3][-1]` returns 3
- [ ] Out of bounds raises error (no crash)
- [ ] All 35 local tests still pass

**Estimated Cycles:** 2

**Expected Impact:**
- Conservative: +3 tests
- Realistic: +4 tests
- Optimistic: +5 tests

## Alternative: Wait and See

If OJ #5 data arrives in next few hours, pivot to data-driven approach (proven pattern from M9.1, M13, M15).

**Signals to Watch:**
- New issue created with OJ results
- Pattern: Issue #98 (OJ #2), #114 (OJ #3), #130 (OJ #4)
- Expected: Issue with test breakdown and failure analysis
