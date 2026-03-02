#include "../../../src/BigInteger.h"
#include <iostream>

int main() {
    // Test case 1: -7 // 2 should be -4
    BigInteger a(-7);
    BigInteger b(2);
    BigInteger result1 = a.floorDiv(b);
    std::cout << "-7 // 2 = " << result1 << " (expected -4)" << std::endl;
    
    // Test case 2: -7 % 2 should be 1
    BigInteger result2 = a % b;
    std::cout << "-7 % 2 = " << result2 << " (expected 1)" << std::endl;
    
    // Test case 3: -5 // 3 should be -2
    BigInteger c(-5);
    BigInteger d(3);
    BigInteger result3 = c.floorDiv(d);
    std::cout << "-5 // 3 = " << result3 << " (expected -2)" << std::endl;
    
    // Test case 4: -5 % 3 should be 1
    BigInteger result4 = c % d;
    std::cout << "-5 % 3 = " << result4 << " (expected 1)" << std::endl;
    
    // Test case 5: 7 // -2 should be -4
    BigInteger e(7);
    BigInteger f(-2);
    BigInteger result5 = e.floorDiv(f);
    std::cout << "7 // -2 = " << result5 << " (expected -4)" << std::endl;
    
    // Test case 6: 7 % -2 should be -1
    BigInteger result6 = e % f;
    std::cout << "7 % -2 = " << result6 << " (expected -1)" << std::endl;
    
    return 0;
}
