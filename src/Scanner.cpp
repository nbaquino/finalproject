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
    std::istringstream iss(input_string);
    std::string token_str;

    while (iss >> token_str) {  // This splits on whitespace
        TokenType type = getTokenType(token_str);

        if (type == TokenType::UNKNOWN) {
            // Handle invalid token
            size_t pos = input_string.find(token_str);
            throw std::invalid_argument("There is an error at position " +
                                      std::to_string(pos) +
                                      ". Invalid token: '" + token_str + "'");
        }

        tokens.push_back(createToken(type, token_str, line));
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
    // Check if it's a keyword
    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        return it->second;
    }

    // Check if it's an identifier (P, Q, TRUE, or FALSE)
    if ((lexeme.length() == 1 && (lexeme[0] == 'P' || lexeme[0] == 'Q')) ||
        lexeme == "TRUE" || lexeme == "FALSE") {
        return TokenType::IDENTIFIER;
    }

    return TokenType::UNKNOWN;
}
