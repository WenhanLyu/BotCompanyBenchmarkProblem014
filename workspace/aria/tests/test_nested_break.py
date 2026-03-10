# Test nested break - only breaks inner loop
i = 0
while i < 3:
    j = 0
    while j < 5:
        if j == 2:
            break
        print(i, j)
        j = j + 1
    i = i + 1
print("done")
