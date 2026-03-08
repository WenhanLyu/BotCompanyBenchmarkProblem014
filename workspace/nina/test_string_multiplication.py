# Test 1: Performance - s = "x" * 100000 completes fast
s = "x" * 100000
print("Test 1 PASS: Large string multiplication completed")

# Test 2a: "x" * 100 works
result1 = "x" * 100
if len(result1) == 100:
    print("Test 2a PASS: String * Integer works")
else:
    print("Test 2a FAIL")

# Test 2b: 100 * "x" works
result2 = 100 * "x"
if len(result2) == 100:
    print("Test 2b PASS: Integer * String works")
else:
    print("Test 2b FAIL")

# Test 3: Augmented assignment s *= 100
s = "x"
s *= 100
if len(s) == 100:
    print("Test 3 PASS: Augmented assignment *= works")
else:
    print("Test 3 FAIL")

# Test 4: Multi-arg print
print("Test 4:", "Multi-arg", "print", "works")

# Test 5: Exception handling
try:
    x = 1 / 0
except:
    print("Test 5 PASS: Exception handling works")
