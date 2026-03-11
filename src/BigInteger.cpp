#include "BigInteger.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <cmath>

// Constructors

BigInteger::BigInteger() : digits(), negative(false) {
    // Default: represents zero (empty vector)
}

BigInteger::BigInteger(long long value) : negative(value < 0) {
    if (value == 0) {
        // Zero: empty vector
        return;
    }
    
    // Work with absolute value
    unsigned long long absValue = (value < 0) ? -(unsigned long long)value : value;
    
    while (absValue > 0) {
        digits.push_back(absValue % BASE);
        absValue /= BASE;
    }
}

BigInteger::BigInteger(const std::string& str) : negative(false) {
    if (str.empty() || !isValidNumber(str)) {
        throw std::invalid_argument("Invalid number string: " + str);
    }
    
    // Parse sign
    size_t startPos = 0;
    if (str[0] == '-') {
        negative = true;
        startPos = 1;
    } else if (str[0] == '+') {
        startPos = 1;
    }
    
    // Remove leading zeros
    while (startPos < str.length() && str[startPos] == '0') {
        startPos++;
    }
    
    // Check if it's zero
    if (startPos >= str.length()) {
        negative = false;
        return;
    }
    
    // Parse digits from right to left in groups of 9
    std::string absStr = str.substr(startPos);
    int len = absStr.length();
    
    for (int i = len; i > 0; i -= BASE_DIGITS) {
        int start = std::max(0, i - BASE_DIGITS);
        int count = i - start;
        std::string digitStr = absStr.substr(start, count);
        digits.push_back(std::stoi(digitStr));
    }
    
    normalize();
}

BigInteger::BigInteger(const BigInteger& other) 
    : digits(other.digits), negative(other.negative) {
}

BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this != &other) {
        digits = other.digits;
        negative = other.negative;
    }
    return *this;
}

// Helper methods

void BigInteger::removeLeadingZeros() {
    while (!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }
}

void BigInteger::normalize() {
    removeLeadingZeros();
    if (digits.empty()) {
        negative = false; // Zero is always positive
    }
}

bool BigInteger::isZero() const {
    return digits.empty();
}

bool BigInteger::isNegative() const {
    return negative && !isZero();
}

int BigInteger::compareAbs(const BigInteger& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() < other.digits.size() ? -1 : 1;
    }
    
    for (int i = digits.size() - 1; i >= 0; i--) {
        if (digits[i] != other.digits[i]) {
            return digits[i] < other.digits[i] ? -1 : 1;
        }
    }
    
    return 0; // Equal
}

std::string BigInteger::toString() const {
    if (isZero()) {
        return "0";
    }
    
    std::string result;
    if (negative) {
        result = "-";
    }
    
    // Most significant digit (no leading zeros)
    result += std::to_string(digits.back());
    
    // Remaining digits (pad to 9 digits)
    for (int i = digits.size() - 2; i >= 0; i--) {
        std::string digitStr = std::to_string(digits[i]);
        result += std::string(BASE_DIGITS - digitStr.length(), '0') + digitStr;
    }
    
    return result;
}

long long BigInteger::toLongLong() const {
    if (isZero()) {
        return 0;
    }
    
    long long result = 0;
    long long multiplier = 1;
    
    for (size_t i = 0; i < digits.size(); i++) {
        result += digits[i] * multiplier;
        multiplier *= BASE;
    }
    
    return negative ? -result : result;
}

bool BigInteger::isValidNumber(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') {
        start = 1;
        if (str.length() == 1) {
            return false; // Just a sign
        }
    }
    
    for (size_t i = start; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    
    return true;
}

// Arithmetic helper methods

BigInteger BigInteger::addAbs(const BigInteger& other) const {
    BigInteger result;
    int carry = 0;
    size_t maxSize = std::max(digits.size(), other.digits.size());
    
    for (size_t i = 0; i < maxSize || carry; i++) {
        long long sum = carry;
        if (i < digits.size()) {
            sum += digits[i];
        }
        if (i < other.digits.size()) {
            sum += other.digits[i];
        }
        
        result.digits.push_back(sum % BASE);
        carry = sum / BASE;
    }
    
    result.normalize();
    return result;
}

BigInteger BigInteger::subtractAbs(const BigInteger& other) const {
    // Assumes this >= other (in absolute value)
    BigInteger result;
    int borrow = 0;
    
    for (size_t i = 0; i < digits.size(); i++) {
        long long diff = digits[i] - borrow;
        if (i < other.digits.size()) {
            diff -= other.digits[i];
        }
        
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.digits.push_back(diff);
    }
    
    result.normalize();
    return result;
}

BigInteger BigInteger::multiplyAbs(const BigInteger& other) const {
    BigInteger result;
    result.digits.resize(digits.size() + other.digits.size(), 0);
    
    // Accumulate products directly into result.digits, propagating carries
    // immediately to prevent overflow in the accumulator
    for (size_t i = 0; i < digits.size(); i++) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < other.digits.size(); j++) {
            unsigned long long product = (unsigned long long)digits[i] * other.digits[j];
            unsigned long long sum = result.digits[i + j] + product + carry;
            result.digits[i + j] = sum % BASE;
            carry = sum / BASE;
        }
        // Propagate any remaining carry
        size_t k = i + other.digits.size();
        while (carry > 0 && k < result.digits.size()) {
            unsigned long long sum = result.digits[k] + carry;
            result.digits[k] = sum % BASE;
            carry = sum / BASE;
            k++;
        }
    }
    
    result.normalize();
    return result;
}

void BigInteger::divideAbs(const BigInteger& divisor, BigInteger& quotient, BigInteger& remainder) const {
    if (divisor.isZero()) {
        throw std::runtime_error("Division by zero");
    }
    
    quotient = BigInteger();
    remainder = BigInteger();
    
    if (isZero()) {
        return;
    }
    
    // Long division algorithm
    for (int i = digits.size() - 1; i >= 0; i--) {
        // Shift remainder left by BASE (multiply by BASE) and add next digit
        // In our positional system, multiplying by BASE means shifting all digits up one position
        if (!remainder.isZero()) {
            // Insert 0 at the beginning (least significant position) and shift all digits up
            remainder.digits.insert(remainder.digits.begin(), 0);
        }
        
        // Add current digit at the least significant position
        if (remainder.isZero()) {
            remainder.digits.push_back(digits[i]);
        } else {
            remainder.digits[0] += digits[i];
            // Handle overflow if the addition causes carry
            if (remainder.digits[0] >= BASE) {
                long long carry = remainder.digits[0] / BASE;
                remainder.digits[0] %= BASE;
                for (size_t j = 1; j < remainder.digits.size() && carry > 0; j++) {
                    remainder.digits[j] += carry;
                    carry = remainder.digits[j] / BASE;
                    remainder.digits[j] %= BASE;
                }
                if (carry > 0) {
                    remainder.digits.push_back(carry);
                }
            }
        }
        
        remainder.normalize();
        
        // Find how many times divisor goes into remainder
        // Use quotient estimation instead of repeated subtraction
        int count = 0;
        
        // Estimate quotient digit based on leading digits
        if (!divisor.isZero() && !remainder.isZero()) {
            long long estimate = 0;
            
            if (remainder.digits.size() == divisor.digits.size()) {
                // Same number of digits - estimate from leading digit
                estimate = remainder.digits.back() / divisor.digits.back();
            } 
            else if (remainder.digits.size() == divisor.digits.size() + 1) {
                // Remainder is one digit longer - use two leading digits
                long long high = remainder.digits.back();
                long long low = (remainder.digits.size() > 1) ? 
                                remainder.digits[remainder.digits.size() - 2] : 0;
                long long dividend_est = high * BASE + low;
                long long divisor_lead = divisor.digits.back();
                estimate = dividend_est / divisor_lead;
            }
            else if (remainder.digits.size() > divisor.digits.size() + 1) {
                // Remainder is much larger than divisor
                estimate = BASE - 1;
            }
            // else: remainder < divisor, quotient digit is 0
            
            // Clamp estimate to valid digit range [0, BASE-1]
            if (estimate >= BASE) estimate = BASE - 1;
            if (estimate < 0) estimate = 0;
            
            // Verify estimate and adjust if needed using binary search
            // This prevents O(n) behavior when estimate is significantly off
            int low = 0;
            int high = (int)estimate;
            count = 0;
            
            while (low <= high) {
                int mid = low + (high - low) / 2;
                
                // Calculate divisor * mid
                BigInteger product(mid);
                product = divisor * product;
                
                // Check if mid is valid
                int cmp = remainder.compareAbs(product);
                if (cmp >= 0) {
                    // mid works, but maybe we can go higher
                    count = mid;
                    low = mid + 1;
                } else {
                    // mid is too high
                    high = mid - 1;
                }
            }
            
            // Subtract the final product
            if (count > 0) {
                BigInteger product(count);
                product = divisor * product;
                remainder = remainder.subtractAbs(product);
            }
        }
        
        quotient.digits.push_back(count);
    }
    
    // Quotient digits are in reverse order
    std::reverse(quotient.digits.begin(), quotient.digits.end());
    quotient.normalize();
    remainder.normalize();
}

// Comparison operators

bool BigInteger::operator==(const BigInteger& other) const {
    if (negative != other.negative) {
        return false;
    }
    return digits == other.digits;
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger& other) const {
    if (negative != other.negative) {
        return negative; // negative < positive
    }
    
    int cmp = compareAbs(other);
    return negative ? (cmp > 0) : (cmp < 0);
}

bool BigInteger::operator<=(const BigInteger& other) const {
    return *this < other || *this == other;
}

bool BigInteger::operator>(const BigInteger& other) const {
    return !(*this <= other);
}

bool BigInteger::operator>=(const BigInteger& other) const {
    return !(*this < other);
}

// Arithmetic operators

BigInteger BigInteger::operator+(const BigInteger& other) const {
    if (negative == other.negative) {
        // Same sign: add absolute values
        BigInteger result = addAbs(other);
        result.negative = negative;
        result.normalize();
        return result;
    } else {
        // Different signs: subtract absolute values
        int cmp = compareAbs(other);
        if (cmp == 0) {
            return BigInteger(); // Zero
        } else if (cmp > 0) {
            BigInteger result = subtractAbs(other);
            result.negative = negative;
            result.normalize();
            return result;
        } else {
            BigInteger result = other.subtractAbs(*this);
            result.negative = other.negative;
            result.normalize();
            return result;
        }
    }
}

BigInteger BigInteger::operator-(const BigInteger& other) const {
    return *this + (-other);
}

BigInteger BigInteger::operator-() const {
    BigInteger result(*this);
    if (!isZero()) {
        result.negative = !negative;
    }
    return result;
}

BigInteger BigInteger::operator+() const {
    return *this;
}

BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger result = multiplyAbs(other);
    result.negative = (negative != other.negative) && !result.isZero();
    result.normalize();
    return result;
}

BigInteger BigInteger::operator/(const BigInteger& other) const {
    BigInteger quotient, remainder;
    divideAbs(other, quotient, remainder);
    
    // Truncating division
    quotient.negative = (negative != other.negative) && !quotient.isZero();
    quotient.normalize();
    return quotient;
}

BigInteger BigInteger::floorDiv(const BigInteger& other) const {
    if (other.isZero()) {
        throw std::runtime_error("Division by zero");
    }
    
    BigInteger quotient, remainder;
    divideAbs(other, quotient, remainder);
    
    // Python floor division floors toward -∞
    bool differentSigns = (negative != other.negative);
    bool hasRemainder = !remainder.isZero();
    
    if (differentSigns && hasRemainder) {
        // Adjust quotient down (toward -∞)
        quotient = quotient + BigInteger(1);
        quotient.negative = true;
    } else {
        quotient.negative = differentSigns && !quotient.isZero();
    }
    
    quotient.normalize();
    return quotient;
}

BigInteger BigInteger::operator%(const BigInteger& other) const {
    if (other.isZero()) {
        throw std::runtime_error("Modulo by zero");
    }
    
    BigInteger quotient, remainder;
    divideAbs(other, quotient, remainder);
    
    // Python modulo: sign of result matches sign of divisor
    bool differentSigns = (negative != other.negative);
    bool hasRemainder = !remainder.isZero();
    
    if (hasRemainder && differentSigns) {
        // Adjust remainder: r = r - |divisor|
        remainder = other.subtractAbs(remainder);
        remainder.negative = other.negative;
    } else {
        remainder.negative = negative && !remainder.isZero();
    }
    
    remainder.normalize();
    return remainder;
}

// I/O operators

std::ostream& operator<<(std::ostream& os, const BigInteger& bi) {
    os << bi.toString();
    return os;
}

std::istream& operator>>(std::istream& is, BigInteger& bi) {
    std::string str;
    is >> str;
    bi = BigInteger(str);
    return is;
}
