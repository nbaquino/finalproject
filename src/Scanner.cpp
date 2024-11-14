#include "../include/Scanner.h"
#include <cctype>
#include <unordered_map>
#include <unordered_set>

// Initialize the set of valid identifiers
std::unordered_set<std::string> validIds = {"P", "Q"}; // Set of valid identifiers for the scanner

// Constructor for the Scanner class, initializes the source string
Scanner::Scanner(const std::string& source) : source(source) {}

// Main function to scan the source and return a list of tokens
std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) { // Continue scanning until the end of the source is reached
        start = current; // Mark the start of the current lexeme
        scanToken();     // Scan the next token
    }

    tokens.push_back(Token(TOKENEOF, "", nullptr, line)); // Add an EOF token at the end
    return tokens; // Return the list of tokens
}

// Check if the scanner has reached the end of the source
bool Scanner::isAtEnd() const {
    return current >= source.length();
}

// advance to the next character in the source and return it
char Scanner::advance() {
    return source[current++];
}

// Peek at the current character without consuming it
char Scanner::peek() const {
    if (isAtEnd()) return '\0'; // Return null character if at the end
    return source[current];
}

// Match the current character with the expected one and advance if it matches
bool Scanner::match(char expected) {
    if (isAtEnd()) return false; // Return false if at the end
    if (source[current] != expected) return false; // Return false if not matching

    current++; // Advance the current position
    return true; // Return true if matched
}

// Skip whitespace characters and handle new lines
void Scanner::skipWhitespace() {
    while (std::isspace(source[current])) { // Check for whitespace
        if (source[current] == '\n') { // Handle new line
            line++; // Increment line number
        }
        current++; // Advance the current position
    }
}

// Scan the next token from the source
void Scanner::scanToken() {
    skipWhitespace(); // Skip any leading whitespace

    char c = advance(); // Get the next character

    // Handle parentheses first - simplest case
    switch (c) {
        case '(':
            addToken(LEFT_PAREN, "("); // Add a left parenthesis token
            return;
        case ')':
            addToken(RIGHT_PAREN, ")"); // Add a right parenthesis token
            return;
    }

    // Handle alphabetic characters (keywords and identifiers)
    if (isalpha(c)) {
        std::string identifier(1, c); // Start building the identifier
        while (isalpha(peek())) { // Continue if the next character is alphabetic
            identifier += advance(); // Add the character to the identifier
        }

        // Check if it's a keyword first
        static const std::unordered_map<std::string, TokenType> keywords = {
            {"AND", AND},
            {"TRUE", TRUE},
            {"FALSE", FALSE},
            {"NOT", NOT},
            {"OR", OR},
            {"IMPLIES", IMPLIES},
            {"EQUIVALENT", EQUIVALENT}
        };

        // Process the identifier
        if (auto it = keywords.find(identifier); it != keywords.end()) {
            addToken(it->second, identifier);  // Keyword found, add corresponding token
        }
        else if (validIds.find(identifier) != validIds.end()) {
            addToken(IDENTIFIER, identifier);  // Valid identifier (P or Q), add token
        }
        else {
            addToken(UNKNOWN, identifier);     // Unknown identifier, add as unknown token
        }
        return;
    }

    // Everything else is unknown
    addToken(UNKNOWN, std::string(1, c)); // Add unknown token for unrecognized characters
}

// Add a token to the list of tokens
void Scanner::addToken(TokenType type, const std::string& lexeme) {
    tokens.push_back(Token(type, lexeme, nullptr, line)); // Create and add a new token
}
