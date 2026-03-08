# Performance test - actually using break
count = 0
i = 0
while i < 1000:
    j = 0
    while j < 100:
        count = count + 1
        j = j + 1
        if j == 50:  # Break at 50, so should get 50 * 1000 = 50000
            break
    i = i + 1
print(count)
