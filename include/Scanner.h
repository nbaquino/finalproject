#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h" // Include the Token class header
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>
#include <unordered_map>

class Scanner {
public:
    // Constructor that accepts the input string to be tokenized
    Scanner(const std::string& input_string);

    // Tokenizes the input string and checks for invalid tokens
    void tokenize();

    // Returns the tokens extracted from the input string
    std::vector<Token> getTokens() const;

private:
    std::string input_string;                 // The original input string
    std::vector<Token> tokens;                // List of tokens extracted from the input string
    int line;                                 // Current line number

    // Helper method to remove spaces from a string
    std::string removeSpaces(const std::string& str);

    // Helper method to find the position of the invalid token
    size_t findInvalidPosition(const std::string& cleaned_input);

    // Helper method to create a Token from a matched string
    Token createToken(TokenType type, const std::string& lexeme, int line);

    static const std::unordered_map<std::string, TokenType> keywords;
    TokenType getTokenType(const std::string& lexeme);
};

#endif // SCANNER_H
