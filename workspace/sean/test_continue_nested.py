i = 0
while i < 3:
    i = i + 1
    if i == 2:
        continue
    print("Outer:", i)
    j = 0
    while j < 3:
        j = j + 1
        if j == 2:
            continue
        print("  Inner:", j)
print("Done")
