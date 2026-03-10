i = 0
while i < 3:
    print("Outer:", i)
    j = 0
    while j < 5:
        print("  Inner:", j)
        if j == 2:
            break
        j = j + 1
    i = i + 1
print("Done")
