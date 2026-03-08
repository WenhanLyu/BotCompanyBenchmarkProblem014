#!/usr/bin/env python3
"""
Edge Case Tests for Defensive Programming Validation
Run with Python 3 to generate expected outputs
"""

print("=== Test 1: INT_MIN Negation ===")
x = -2147483648
y = -x
print(y)
print(type(y))

print("\n=== Test 2: INT_MIN Unary Negation ===")
print(-(-2147483648))

print("\n=== Test 3: Negative String Repetition ===")
s = "abc" * -5
print(s)
print(len(s))
print(repr(s))

print("\n=== Test 4: Zero String Repetition ===")
s = "test" * 0
print(s)
print(len(s))

print("\n=== Test 5: String Repetition Edge Cases ===")
print("a" * 1)
print("a" * -1)
print("" * 5)
print("" * -5)

print("\n=== Test 6: Division by Zero (would crash without fix) ===")
try:
    x = 10 // 0
    print(x)
except ZeroDivisionError as e:
    print(f"ZeroDivisionError: {e}")

print("\n=== Test 7: Modulo by Zero (would crash without fix) ===")
try:
    x = 10 % 0
    print(x)
except ZeroDivisionError as e:
    print(f"ZeroDivisionError: {e}")

print("\n=== Test 8: Float Division by Zero ===")
try:
    x = 10.0 / 0.0
    print(x)
except ZeroDivisionError as e:
    print(f"ZeroDivisionError: {e}")

print("\n=== Test 9: Variable Division by Zero ===")
try:
    y = 0
    z = 100 // y
    print(z)
except ZeroDivisionError as e:
    print(f"ZeroDivisionError: {e}")

print("\n=== Test 10: Augmented Assignment Division by Zero ===")
try:
    x = 10
    x //= 0
    print(x)
except ZeroDivisionError as e:
    print(f"ZeroDivisionError: {e}")

print("\n=== Test 11: Complex Expression with Zero ===")
try:
    x = 5 - 5
    y = 100 // x
    print(y)
except ZeroDivisionError as e:
    print(f"ZeroDivisionError: {e}")

print("\n=== Test 12: INT_MIN Division ===")
x = -2147483648
y = x // -1
print(y)
print(type(y))

print("\n=== Test 13: INT_MIN Modulo ===")
x = -2147483648
y = x % -1
print(y)

print("\n=== Test 14: Large Negative Times Small Negative ===")
x = -2147483648
y = x * -1
print(y)
print(type(y))

print("\n=== Test 15: Overflow in Addition ===")
x = 2147483647
y = x + 1
print(y)
print(type(y))

print("\n=== Test 16: Overflow in Subtraction ===")
x = -2147483648
y = x - 1
print(y)
print(type(y))

print("\n=== Test 17: Overflow in Multiplication ===")
x = 2147483647
y = x * 2
print(y)
print(type(y))

print("\n=== Test 18: String and Int Multiplication ===")
s = "abc" * 3
print(s)
s = "test" * 1
print(s)
s = "x" * 10
print(s)

print("\n=== Test 19: Floor Division with Doubles ===")
x = 7.8 // 2.3
print(x)
print(type(x))

print("\n=== Test 20: Floor Division Sign Handling ===")
print(7 // 2)    # 3
print(-7 // 2)   # -4 (Python floors toward -inf)
print(7 // -2)   # -4
print(-7 // -2)  # 3

print("\n=== Test 21: Modulo Sign Handling ===")
print(7 % 2)     # 1
print(-7 % 2)    # 1 (Python: result has sign of divisor)
print(7 % -2)    # -1
print(-7 % -2)   # -1

print("\n=== Test 22: Combined Operations ===")
x = 10
x += 5
x *= 2
x //= 3
x %= 7
print(x)

print("\n=== Test 23: String Augmented Assignment ===")
s = "hello"
s += " world"
print(s)
s *= 2
print(s)

print("\n=== Test 24: BigInteger Boundary ===")
# Python automatically uses arbitrary precision
x = 10 ** 100
print(x)
y = x + 1
print(y)

print("\n=== Test 25: Unary Plus ===")
x = +5
print(x)
x = +(-5)
print(x)
x = +(2147483647)
print(x)

print("\n=== TESTS COMPLETE ===")
