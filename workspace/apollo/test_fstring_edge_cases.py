# Test edge cases for f-strings

# Test 1: Empty f-string
print(f"")

# Test 2: Only expression
print(f"{42}")

# Test 3: Multiple expressions
print(f"{1}{2}{3}")

# Test 4: All value types
x = 123
y = 3.14
z = True
w = "test"
print(f"int:{x} float:{y} bool:{z} str:{w} none:{None}")

# Test 5: Expression with string concatenation
print(f"Result: {"hello" + " " + "world"}")

# Test 6: Arithmetic in expression
print(f"Math: {10 + 20 * 3}")

# Test 7: Nested f-string (from test15)
print(f"{ f"nested" } works")

# Test 8: BigInteger support
big = 123456789012345678901234567890
print(f"BigInt: {big}")
