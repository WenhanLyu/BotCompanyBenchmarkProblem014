# Test edge cases for multiple return values

# Test 1: Print tuple directly
def get_pair():
    return 1, 2

print(get_pair())

# Test 2: Assign tuple to single variable
def get_triple():
    return 10, 20, 30

t = get_triple()
print(t)

# Test 3: Multiple returns in different branches
def conditional_return(x):
    if x > 0:
        return 1, 2
    else:
        return 3, 4

a, b = conditional_return(5)
print(a)
print(b)

c, d = conditional_return(-1)
print(c)
print(d)
