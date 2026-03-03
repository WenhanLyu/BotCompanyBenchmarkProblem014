# Test evaluation order in tuple assignment
counter = 0

def next_val():
    global counter
    counter += 1
    return counter

# Python evaluates left to right
a, b = next_val(), next_val()
print(a)  # Should be 1
print(b)  # Should be 2
