#include "../include/Scanner.h"
#include <unordered_map>
#include <sstream>

// Add this at the class level (in Scanner.h or at the top of Scanner.cpp)
const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"TRUE", TokenType::TRUE},
    {"FALSE", TokenType::FALSE},
    {"NOT", TokenType::NOT},
    {"AND", TokenType::AND},
    {"OR", TokenType::OR},
    {"IMPLIES", TokenType::IMPLIES},
    {"EQUIVALENT", TokenType::EQUIVALENT},
    {"(", TokenType::LEFT_PAREN},
    {")", TokenType::RIGHT_PAREN}
};

// Constructor that accepts the input string to be tokenized
Scanner::Scanner(const std::string& input_string) : input_string(input_string), line(1) {
    tokenize();
}

// Tokenizes the input string and checks for invalid tokens
void Scanner::tokenize() {
    size_t current = 0;

    while (current < input_string.length()) {
        // Skip whitespace
        if (std::isspace(input_string[current])) {
            if (input_string[current] == '\n') line++;
            current++;
            continue;
        }

        // Handle parentheses
        if (input_string[current] == '(' || input_string[current] == ')') {
            std::string lexeme(1, input_string[current]);
            tokens.push_back(createToken(getTokenType(lexeme), lexeme, line));
            current++;
            continue;
        }

        // Handle identifiers and keywords
        if (std::isalpha(input_string[current])) {
            size_t start = current;
            while (current < input_string.length() && std::isalpha(input_string[current])) {
                current++;
            }
            std::string lexeme = input_string.substr(start, current - start);
            TokenType type = getTokenType(lexeme);

            if (type == TokenType::UNKNOWN) {
                throw std::invalid_argument("There is an error at position " +
                                          std::to_string(start) +
                                          ". Invalid token: '" + lexeme + "'");
            }

            tokens.push_back(createToken(type, lexeme, line));
            continue;
        }

        // If we get here, we've encountered an invalid character
        throw std::invalid_argument("There is an error at position " +
                                  std::to_string(current) +
                                  ". Invalid character: '" + std::string(1, input_string[current]) + "'");
    }
}

// Returns the tokens extracted from the input string
std::vector<Token> Scanner::getTokens() const {
    return tokens;
}

// Helper method to create a Token from a matched string
Token Scanner::createToken(TokenType type, const std::string& lexeme, int line) {
    return Token(type, lexeme, nullptr, line); // For simplicity, we pass nullptr as the literal value
}

// Add this new helper method
TokenType Scanner::getTokenType(const std::string& lexeme) {
    // Handle parentheses
    if (lexeme == "(") return TokenType::LEFT_PAREN;
    if (lexeme == ")") return TokenType::RIGHT_PAREN;

    // Check if it's a single-letter identifier (P or Q)
    if (lexeme.length() == 1 && (lexeme[0] == 'P' || lexeme[0] == 'Q')) {
        return TokenType::IDENTIFIER;
    }

    // Check keywords map
    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        return it->second;
    }

    return TokenType::UNKNOWN;
}
