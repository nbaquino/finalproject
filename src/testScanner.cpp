#include "../include/Scanner.h"
#include <iostream>
#include <vector>

// Function to print the tokens generated by the scanner
void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Token(Type: " << token.type
                  << ", Lexeme: \"" << token.lexeme
                  << "\", Line: " << token.line << ")\n";
    }
}

int main() {
    // Test input strings
    std::vector<std::string> testInputs = {
        "(P AND Q)",
        "TRUE OR FALSE",
        "NOT (P IMPLIES Q)",
        "P EQUIVALENT Q",
        "UNKNOWN_TOKEN"
    };

    // Iterate over each test input
    for (const auto& input : testInputs) {
        std::cout << "Testing input: \"" << input << "\"\n";

        // Create a Scanner instance
        Scanner scanner(input);

        // Scan the tokens
        std::vector<Token> tokens = scanner.scanTokens();

        // Print the tokens
        printTokens(tokens);

        std::cout << "-------------------------\n";
    }

    return 0;
}
