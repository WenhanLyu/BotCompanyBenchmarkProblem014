i = 0
while i < 3:
    print("Level 1:", i)
    j = 0
    while j < 3:
        j = j + 1
        if j == 2:
            continue
        k = 0
        while k < 3:
            k = k + 1
            if k == 2:
                break
            print("  Level 3:", i, j, k)
    i = i + 1
print("Complete")
