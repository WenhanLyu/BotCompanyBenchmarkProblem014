# Nested loops with continue
i = 0
while i < 3:
    j = 0
    while j < 5:
        j = j + 1
        if j == 3:
            continue
        print(i * 10 + j)
    i = i + 1
print("done")
