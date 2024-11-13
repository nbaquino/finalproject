#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cctype>

// Set of valid identifier names used in propositional logic
std::unordered_set<std::string> validIds = {"P", "Q"};

// Token types for the lexical analyzer
enum TokenType {
    // Single character
    LEFT_PAREN,
    RIGHT_PAREN,

    // Literals (identifiers or variables)
    IDENTIFIER,

    // Keywords
    AND,
    TRUE,
    FALSE,
    NOT,
    OR,
    IMPLIES,
    EQUIVALENT,
    TOKENEOF,
    UNKNOWN
};

// Represents a token with its type, lexeme, literal value, and line number
class Token {
public:
    const TokenType type;
    const std::string lexeme;
    const std::shared_ptr<void> literal;
    int line;

    // Constructor
    Token(TokenType type, const std::string& lexeme, std::shared_ptr<void> literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}
};

/**
 * Scanner Class - Performs lexical analysis on propositional logic expressions
 */
class Scanner {
private:
    const std::string source;      // Input source code to be scanned
    std::vector<Token> tokens;     // Collection of tokens found during scanning
    int start = 0;                 // Start position of current lexeme
    int current = 0;               // Current position in source
    int line = 1;                  // Current line number being processed

public:
    // Constructor - Initializes scanner with source code to be analyzed
    explicit Scanner(const std::string& source)
        : source(source) {}

    /**
     * Scans the entire source code and returns a vector of tokens
     * @return Vector of tokens found in the source
     */
    std::vector<Token> scanTokens() {
        while (!isAtEnd()) {
            // We are at the beginning of the next lexeme.
            start = current;
            scanToken();
        }

        tokens.push_back(Token(TOKENEOF, "", nullptr, line));  // Adds an EOF token
        return tokens;
    }

private:
    /**
     * Checks if scanner has reached the end of source
     * @return true if at end of source, false otherwise
     */
    bool isAtEnd() const {
        return current >= source.length();
    }

    /**
     * Consumes and returns the current character, advancing the position
     * @return The current character
     */
    char move() {
        return source[current++];
    }

    /**
     * Returns the current character without consuming it
     * @return The current character or '\0' if at end
     */
    char peek() const {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    /**
     * Checks if the current character matches the expected character
     * @param expected The character to match against
     * @return true if matches and consumed, false otherwise
     */
    bool match(char expected) {
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;

        current++;
        return true;
    }

    /**
     * Skips whitespace characters and updates line count
     */
    void skipWhitespace() {
        while (std::isspace(source[current])) {
            if (source[current] == '\n') {
                line++;
            }
            current++;
        }
    }

    /**
     * Scans and processes the next token in the source
     */
    void scanToken() {
        skipWhitespace();

        char c = move();
        if (isalpha(c)) {
            // Start of an identifier or keyword
            std::string identifier(1, c);
            while (isalpha(peek())) {
                identifier += move();
            }

            // Unordered map for keywords, operators, and parentheses defined inside scanToken
            const std::unordered_map<std::string, TokenType> keywordMap = {
                {"AND", AND},
                {"TRUE", TRUE},
                {"FALSE", FALSE},
                {"NOT", NOT},
                {"OR", OR},
                {"IMPLIES", IMPLIES},
                {"EQUIVALENT", EQUIVALENT},
                {"(", LEFT_PAREN},
                {")", RIGHT_PAREN}
            };

            // Check if it's a known keyword or identifier
            auto it = keywordMap.find(identifier);
            if (it != keywordMap.end()) {
                addToken(it->second, identifier);
            } else if (validIds.find(identifier) != validIds.end()) {
                addToken(IDENTIFIER, identifier);
            } else {
                addToken(UNKNOWN, identifier);
            }
        } else if (c == '(') {
            addToken(LEFT_PAREN, "(");
        } else if (c == ')') {
            addToken(RIGHT_PAREN, ")");
        } else {
            // Handle unknown characters
            addToken(UNKNOWN, std::string(1, c));
        }
    }

    /**
     * Creates and adds a new token to the tokens vector
     * @param type The type of token
     * @param lexeme The string value of the token
     */
    void addToken(TokenType type, const std::string& lexeme) {
        tokens.push_back(Token(type, lexeme, nullptr, line));
    }
};
