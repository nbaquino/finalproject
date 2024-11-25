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

/**
 * @brief Constructs a Scanner object to tokenize the input string
 * @param input_string The logical expression to be tokenized
 */
Scanner::Scanner(const std::string& input_string) : input_string(input_string), line(1) {
    tokenize();
}

/**
 * @brief Processes the input string and converts it into tokens
 * @throws std::invalid_argument if an invalid token or character is encountered
 */
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

/**
 * @brief Returns the vector of tokens generated from the input string
 * @return std::vector<Token> The list of tokens
 */
std::vector<Token> Scanner::getTokens() const {
    return tokens;
}

/**
 * @brief Creates a new Token object with the given parameters
 * @param type The TokenType of the token
 * @param lexeme The string value of the token
 * @param line The line number where the token appears
 * @return Token The created token object
 */
Token Scanner::createToken(TokenType type, const std::string& lexeme, int line) {
    return Token(type, lexeme, nullptr, line); // For simplicity, we pass nullptr as the literal value
}

/**
 * @brief Determines the TokenType for a given lexeme
 * @param lexeme The string to be classified
 * @return TokenType The type of the token (UNKNOWN if not recognized)
 */
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
