#pragma once
#ifndef PYTHON_INTERPRETER_BIGINTEGER_H
#define PYTHON_INTERPRETER_BIGINTEGER_H

#include <vector>
#include <string>
#include <iostream>

/**
 * BigInteger class for arbitrary precision integer arithmetic.
 * 
 * Design Philosophy:
 * - Supports integers with thousands of digits (requirement from BigIntegerTest cases)
 * - Uses vector of digits for storage (base 10^9 for efficiency)
 * - Implements Python-compatible floor division (floors toward -∞)
 * - Handles both positive and negative numbers
 * - Integrates with Value type system using std::variant
 * 
 * Storage Strategy:
 * - Vector stores digits in reverse order (least significant first)
 * - Each element is a "digit" in base 10^9 (0 to 999,999,999)
 * - Base 10^9 chosen for:
 *   1. Efficient storage (32-bit int can hold up to ~2.1B)
 *   2. Fast multiplication (two 10^9 values fit in 64-bit)
 *   3. Reasonable conversion to/from decimal strings
 * - Separate sign flag (true = negative, false = positive/zero)
 * - Zero is represented as empty vector with sign = false
 * 
 * Example: 1234567890123456789
 * - Stored as: [567890123, 234, 1] with sign = false
 * - Represents: 1 * 10^18 + 234 * 10^9 + 567890123
 * 
 * Python Floor Division Semantics:
 * - Python's // operator always floors toward negative infinity
 * - Unlike C++ integer division which truncates toward zero
 * - Examples:
 *   *  7 //  2 =  3  (same as C++)
 *   * -7 //  2 = -4  (C++ gives -3)
 *   *  7 // -2 = -4  (C++ gives -3)
 *   * -7 // -2 =  3  (same as C++)
 * - Implementation: if remainder is non-zero and signs differ, adjust quotient down
 */
class BigInteger {
public:
    // Constructors
    BigInteger();                          // Default: creates 0
    BigInteger(long long value);           // From standard integer
    BigInteger(const std::string& str);    // From decimal string
    BigInteger(const BigInteger& other);   // Copy constructor
    
    // Assignment
    BigInteger& operator=(const BigInteger& other);
    
    // Arithmetic operators (return new BigInteger)
    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;  // Truncating division (for reference)
    BigInteger operator%(const BigInteger& other) const;  // Modulo
    
    // Python-style floor division (floors toward -∞)
    BigInteger floorDiv(const BigInteger& other) const;
    
    // Unary operators
    BigInteger operator-() const;  // Negation
    BigInteger operator+() const;  // Unary plus (returns copy)
    
    // Comparison operators
    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;
    
    // Conversion methods
    std::string toString() const;          // Convert to decimal string
    long long toLongLong() const;          // Convert to long long (if fits)
    bool isZero() const;                   // Check if value is zero
    bool isNegative() const;               // Check if value is negative
    bool fitsInInt() const;                // Check if value fits in int (32-bit signed)
    
    // I/O operators
    friend std::ostream& operator<<(std::ostream& os, const BigInteger& bi);
    friend std::istream& operator>>(std::istream& is, BigInteger& bi);

private:
    // Storage
    static const int BASE = 1000000000;    // 10^9
    static const int BASE_DIGITS = 9;      // Number of decimal digits per element
    
    std::vector<int> digits;               // Digits in base 10^9 (least significant first)
    bool negative;                         // Sign: true = negative, false = positive/zero
    
    // Internal helper methods
    void removeLeadingZeros();             // Remove leading zero digits
    void normalize();                      // Normalize representation (remove zeros, fix sign)
    
    // Comparison helpers (ignoring sign)
    int compareAbs(const BigInteger& other) const;  // Compare absolute values
    
    // Arithmetic helpers (for implementation)
    BigInteger addAbs(const BigInteger& other) const;      // Add absolute values
    BigInteger subtractAbs(const BigInteger& other) const; // Subtract absolute values (this >= other)
    BigInteger multiplyAbs(const BigInteger& other) const; // Multiply absolute values
    void divideAbs(const BigInteger& divisor, BigInteger& quotient, BigInteger& remainder) const;
    
    // String conversion helpers
    static bool isValidNumber(const std::string& str);  // Validate numeric string
};

#endif // PYTHON_INTERPRETER_BIGINTEGER_H
