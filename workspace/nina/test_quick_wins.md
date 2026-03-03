# Quick Wins Testing Report

## Test 1: Multi-argument print() - Fix #75

**Test Case 1a: Multiple integers**
```python
print(1, 2, 3)
```
Expected output: `1 2 3`

**Test Case 1b: Mixed types**
```python
print("Hello", 42, "World")
```
Expected output: `Hello 42 World`

**Test Case 1c: Single argument (no regression)**
```python
print(100)
```
Expected output: `100`

---

## Test 2: Exception Handling - Fix #76

**Test Case 2a: Division by zero**
```python
x = 10 / 0
```
Expected: Program catches exception, doesn't crash with SIGABRT

**Test Case 2b: Integer division by zero**
```python
x = 10 // 0
```
Expected: Program catches exception, doesn't crash with SIGABRT

**Test Case 2c: Modulo by zero**
```python
x = 10 % 0
```
Expected: Program catches exception, doesn't crash with SIGABRT

---

## Test 3: String *= Performance - Fix #77

**Test Case 3a: Large string repetition**
```python
s = "x" * 100000
print(len(s))
```
Expected: Completes in < 1 second, outputs `100000`

**Test Case 3b: Augmented assignment version**
```python
s = "x"
s *= 100000
print(len(s))
```
Expected: Completes in < 1 second, outputs `100000`

