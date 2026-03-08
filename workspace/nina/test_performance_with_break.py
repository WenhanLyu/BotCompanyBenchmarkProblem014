# Performance test - loops with break/continue
count = 0
i = 0
while i < 1000:
    j = 0
    while j < 100:
        count = count + 1
        j = j + 1
        if j == 101:  # This will never be true, so no break
            break
    i = i + 1
    if i == 1001:  # This will never be true, so no break
        break
print(count)
