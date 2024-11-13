#include "Parser.cpp"
#include <iostream>

void testParser(const std::string& input) {
    // Create scanner and get tokens
    Scanner scanner(input);
    std::vector<Token> tokens = scanner.scanTokens();

    // Create parser
    Parser parser(tokens);
    std::shared_ptr<Expr> expression = parser.parse();

    if (expression == nullptr) {
        std::cout << "Error parsing: " << input << std::endl;
    } else {
        std::cout << "Successfully parsed: " << input << std::endl;
    }
}

int main() {
    // Test cases
    testParser("P AND Q");
    testParser("NOT P");
    testParser("P OR Q AND S");
    testParser("(P AND Q) OR S");
    testParser("P IMPLIES Q");
    testParser("P EQUIVALENT Q");
    testParser("P AND (Q OR S)");

    // Test invalid cases
    testParser("P AND");  // Should fail
    testParser("AND P");  // Should fail
    testParser("(P AND Q");  // Should fail - missing parenthesis

    return 0;
} 
