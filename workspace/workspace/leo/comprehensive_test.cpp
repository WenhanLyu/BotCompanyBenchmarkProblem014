#include "../../../src/BigInteger.h"
#include <iostream>

void test(long long a_val, long long b_val, long long expected_floor, long long expected_mod) {
    BigInteger a(a_val);
    BigInteger b(b_val);
    BigInteger floor_result = a.floorDiv(b);
    BigInteger mod_result = a % b;
    
    bool floor_ok = (floor_result == BigInteger(expected_floor));
    bool mod_ok = (mod_result == BigInteger(expected_mod));
    
    std::cout << a_val << " // " << b_val << " = " << floor_result 
              << " (expected " << expected_floor << ") " 
              << (floor_ok ? "✓" : "✗") << std::endl;
    std::cout << a_val << " % " << b_val << " = " << mod_result 
              << " (expected " << expected_mod << ") " 
              << (mod_ok ? "✓" : "✗") << std::endl;
    
    // Verify the relationship: a = q * b + r
    BigInteger q = floor_result;
    BigInteger r = mod_result;
    BigInteger reconstructed = q * b + r;
    bool relationship_ok = (reconstructed == a);
    std::cout << "Relationship a = q*b + r: " << (relationship_ok ? "✓" : "✗") << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "Testing BigInteger floor division and modulo" << std::endl;
    std::cout << "=============================================" << std::endl << std::endl;
    
    // Positive / Positive
    test(7, 2, 3, 1);
    test(8, 2, 4, 0);
    
    // Negative / Positive
    test(-7, 2, -4, 1);
    test(-8, 2, -4, 0);
    test(-5, 3, -2, 1);
    
    // Positive / Negative
    test(7, -2, -4, -1);
    test(8, -2, -4, 0);
    test(5, -3, -2, -1);
    
    // Negative / Negative
    test(-7, -2, 3, -1);
    test(-8, -2, 4, 0);
    test(-5, -3, 1, -2);
    
    return 0;
}
