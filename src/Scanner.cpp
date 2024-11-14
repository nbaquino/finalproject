#include "../include/Scanner.h"
#include <cctype>
#include <unordered_map>
#include <unordered_set>

// Constructor for the Scanner class, initializes the source string
/*
 * @param source
 * The string to be scanned
 */
Scanner::Scanner(const std::string& source) : source(source) {}

// Main function to scan the source and return a list of tokens
/*
 * @return
 * A vector of tokens parsed from the source
 */
std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.push_back(Token(TokenType::TOKENEOF, "", nullptr, line));
    return tokens;
}

// Tokenizes the entire source code. This function is similar to scanTokens but kept separate for specific reasons.
/*
 * @return
 * A vector of tokens representing the entire source code
 */
std::vector<Token> Scanner::tokenize() {
    while (current < source.length()) {
        start = current;
        scanToken();
    }
    addToken(TokenType::TOKENEOF, "");
    return tokens;
}

// Check if the scanner has reached the end of the source
/*
 * @return
 * True if the end of the source has been reached, false otherwise
 */
bool Scanner::isAtEnd() const {
    return current >= source.length();
}

// Advance to the next character in the source and return it
/*
 * @return
 * The next character in the source
 */
char Scanner::advance() {
    return source[current++];
}

// Peek at the current character without consuming it
/*
 * @return
 * The current character in the source without advancing the scanner
 */
char Scanner::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

// Match the current character with the expected one and advance if it matches
/*
 * @param expected
 * The character expected to match the current character in the source
 * @return
 * True if the current character matches the expected character, false otherwise
 */
bool Scanner::match(char expected) {
    if (isAtEnd() || source[current] != expected) return false;
    current++;
    return true;
}

// Initialize the set of valid identifiers
std::unordered_set<std::string> validIds = {"P", "Q"};

// Skip whitespace characters and handle new lines
void Scanner::skipWhitespace() {
    while (std::isspace(source[current])) {
        if (source[current] == '\n') line++;
        current++;
    }
}

// Scan the next token from the source
void Scanner::scanToken() {
    skipWhitespace();
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN, "("); return;
        case ')': addToken(TokenType::RIGHT_PAREN, ")"); return;
    }
    if (isalpha(c)) {
        std::string identifier(1, c);
        while (isalpha(peek())) identifier += advance();
        static const std::unordered_map<std::string, TokenType> keywords = {
            {"AND", TokenType::AND},
            {"TRUE", TokenType::TRUE},
            {"FALSE", TokenType::FALSE},
            {"NOT", TokenType::NOT},
            {"OR", TokenType::OR},
            {"IMPLIES", TokenType::IMPLIES},
            {"EQUIVALENT", TokenType::EQUIVALENT}
        };
        if (auto it = keywords.find(identifier); it != keywords.end()) {
            addToken(it->second, identifier);
        } else if (validIds.find(identifier) != validIds.end()) {
            addToken(TokenType::IDENTIFIER, identifier);
        } else {
            addToken(TokenType::UNKNOWN, identifier);
        }
        return;
    }
    addToken(TokenType::UNKNOWN, std::string(1, c));
}

// Adds a token to the list of tokens with the given type and lexeme
/*
 * @param type
 * The type of the token to add
 * @param lexeme
 * The lexeme of the token to add
 */
void Scanner::addToken(TokenType type, const std::string& lexeme) {
    tokens.push_back(Token(type, lexeme, nullptr, line));
}
