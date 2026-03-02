// Test file to verify Value type system
#include <variant>
#include <string>
#include <iostream>

// Replicate the Value definition
using Value = std::variant<std::monostate, std::string>;

// Replicate the unquoteString function
std::string unquoteString(const std::string& str) {
    if (str.length() >= 2) {
        if ((str.front() == '"' && str.back() == '"') ||
            (str.front() == '\'' && str.back() == '\'')) {
            return str.substr(1, str.length() - 2);
        }
    }
    return str;
}

int main() {
    std::cout << "=== Value Type System Verification Tests ===" << std::endl;
    
    // Test 1: std::variant basics
    std::cout << "\n1. Testing std::variant-based Value class:" << std::endl;
    Value v1 = std::monostate{};
    std::cout << "   - Empty value (monostate): " << (std::holds_alternative<std::monostate>(v1) ? "PASS" : "FAIL") << std::endl;
    
    Value v2 = std::string("Hello");
    std::cout << "   - String value: " << (std::holds_alternative<std::string>(v2) ? "PASS" : "FAIL") << std::endl;
    std::cout << "   - String content: " << std::get<std::string>(v2) << std::endl;
    
    // Test 2: unquoteString function
    std::cout << "\n2. Testing unquoteString() function:" << std::endl;
    
    std::string test1 = "\"Hello World\"";
    std::string result1 = unquoteString(test1);
    std::cout << "   - Double quotes: \"" << test1 << "\" -> \"" << result1 << "\" " 
              << (result1 == "Hello World" ? "PASS" : "FAIL") << std::endl;
    
    std::string test2 = "'Single quoted'";
    std::string result2 = unquoteString(test2);
    std::cout << "   - Single quotes: \"" << test2 << "\" -> \"" << result2 << "\" " 
              << (result2 == "Single quoted" ? "PASS" : "FAIL") << std::endl;
    
    std::string test3 = "\"\"";
    std::string result3 = unquoteString(test3);
    std::cout << "   - Empty string: \"" << test3 << "\" -> \"" << result3 << "\" " 
              << (result3 == "" ? "PASS" : "FAIL") << std::endl;
    
    std::string test4 = "NoQuotes";
    std::string result4 = unquoteString(test4);
    std::cout << "   - No quotes: \"" << test4 << "\" -> \"" << result4 << "\" " 
              << (result4 == "NoQuotes" ? "PASS" : "FAIL") << std::endl;
    
    std::string test5 = "\"";
    std::string result5 = unquoteString(test5);
    std::cout << "   - Single char: \"" << test5 << "\" -> \"" << result5 << "\" " 
              << (result5 == "\"" ? "PASS" : "FAIL") << std::endl;
    
    std::string test6 = "\"Mismatched'";
    std::string result6 = unquoteString(test6);
    std::cout << "   - Mismatched quotes: \"" << test6 << "\" -> \"" << result6 << "\" " 
              << (result6 == "\"Mismatched'" ? "PASS" : "FAIL") << std::endl;
    
    std::cout << "\n=== All Tests Complete ===" << std::endl;
    
    return 0;
}
