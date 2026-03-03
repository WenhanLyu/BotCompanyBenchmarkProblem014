# Test Cases for SIGABRT Investigation
# These tests will crash the current implementation

# Test 1: Simple integer division by zero
print(10 // 0)

# Test 2: Simple integer modulo by zero  
print(10 % 0)

# Test 3: Variable division by zero
x = 10
y = 0
print(x // y)

# Test 4: Computed zero divisor
a = 5
b = 5
print(100 // (a - b))

# Test 5: Augmented assignment division by zero
x = 50
x //= 0

# Test 6: Augmented assignment modulo by zero
y = 30
y %= 0

# Test 7: BigInteger division by zero (if Python literals work)
# Note: Current parser has issues with multi-line, but conceptually:
# x = 99999999999999999999
# print(x // 0)

# Test 8: Negative division by zero
print(-10 // 0)

# Test 9: Expression with division by zero
result = (100 + 50) // (10 - 10)

# Test 10: Chain of operations
x = 10
y = 5
z = y - 5
print(x // z)

# All of these will currently produce:
# libc++abi: terminating due to uncaught exception of type std::runtime_error: Division by zero
# Exit with SIGABRT (Signal 6)

# After fix, they should:
# - Exit cleanly without crash
# - Optionally print error message matching Python format
