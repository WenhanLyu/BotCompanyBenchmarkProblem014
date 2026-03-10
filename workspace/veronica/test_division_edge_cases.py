# Test edge cases for BigInteger division

test_cases = [
    # (dividend, divisor, expected_quotient, expected_remainder)
    ("0", "1", "0", "0"),
    ("1", "1", "1", "0"),
    ("10", "1", "10", "0"),
    ("10", "3", "3", "1"),
    ("100", "10", "10", "0"),
    ("999999999999999999", "1", "999999999999999999", "0"),
    ("1000000000000000000", "999999999999999999", "1", "1"),
    ("10", "100", "0", "10"),  # dividend < divisor
    ("123456789", "987654321", "0", "123456789"),  # dividend < divisor
    # Large dividend, small divisor
    ("99999999999999999999999999999999999999999999", "3", "33333333333333333333333333333333333333333333", "0"),
    # Powers of 10
    ("1000000000000000000", "1000000000", "1000000000", "0"),
    # Near BASE boundary (BASE = 10^9)
    ("999999999", "999999998", "1", "1"),
    ("1000000000", "999999999", "1", "1"),
    ("1000000001", "1000000000", "1", "1"),
    # Test with BASE-1 as divisor
    ("999999998999999998", "999999999", "999999999", "999999999"),
]

for dividend, divisor, exp_quot, exp_rem in test_cases:
    print(f"a = {dividend}")
    print(f"b = {divisor}")
    print(f"print(a // b)")
    print(f"print(a % b)")
    print()
