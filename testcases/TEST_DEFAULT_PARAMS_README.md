# Default Parameters Test Suite

## Overview
Comprehensive test suite for default parameter functionality as part of Milestone 21 (M21).

## Files
- `test_default_params.py` - Main test file with 22 test cases
- `test_default_params.out` - Expected output
- `../test_default_params.sh` - Test runner script

## Test Coverage

### 1. Basic Functionality
- **Test 1**: Basic default parameter (1 required, 1 optional)
- **Test 2**: Multiple default parameters (1 required, 3 optional)
- **Test 3**: All parameters have defaults (0 required, 2 optional)

### 2. Expression Defaults
- **Test 4**: Simple arithmetic expressions (1+1)
- **Test 5**: Multiplication expressions (3*2)
- **Test 15**: Complex expression defaults (multiple operations)

### 3. Type Coverage
- **Test 6**: Integer defaults
- **Test 7**: Float defaults
- **Test 8**: String defaults
- **Test 9**: Boolean defaults
- **Test 10**: List defaults
- **Test 11**: Mixed types (string, int, float)

### 4. Edge Cases
- **Test 12**: Negative number defaults
- **Test 13**: Zero defaults
- **Test 14**: Empty string default
- **Test 16**: Many parameters (5 defaults)
- **Test 20**: Large number default (1000000)

### 5. Advanced Scenarios
- **Test 17**: Function name reuse with different defaults
- **Test 18**: Defaults in recursive functions
- **Test 19**: Mixed required and optional parameters (2 required, 3 optional)
- **Test 21**: Single parameter all optional
- **Test 22**: String operations with defaults

## Running Tests

### Quick Test
```bash
./test_default_params.sh
```

### Manual Test
```bash
./code < testcases/test_default_params.py
```

### Compare Output
```bash
./code < testcases/test_default_params.py > actual.out
diff testcases/test_default_params.out actual.out
```

## Expected Results
All 22 tests should pass with no output differences.

## Implementation Notes
- Tuple literals in default parameters are not supported (parse errors)
- Default expressions referencing previous parameters (e.g., `b=a+1`) are not supported
- Mutable default behavior (list defaults) may differ from standard Python
- All basic default parameter use cases are fully functional

## Integration
This test suite is part of M21 Part A (Default Parameters) and should be run alongside:
- Basic M1 tests (`test_m1.sh`)
- BigInteger tests
- Other M21 component tests
