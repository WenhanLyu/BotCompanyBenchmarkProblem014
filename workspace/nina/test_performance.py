# Performance test - loops without break/continue
# This should have minimal overhead from exception handling

count = 0
i = 0
while i < 1000:
    j = 0
    while j < 100:
        count = count + 1
        j = j + 1
    i = i + 1
print(count)
