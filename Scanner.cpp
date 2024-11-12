#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cctype>  // For isdigit(), isalpha(), etc.

enum TokenType {
    // Single character tokens
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
    TOKENEOF,  // Changed to TOKENEOF to avoid conflict with C++ EOF
    UNKNOWN
};

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
* Scanner Class
*/
class Scanner {
private:
    const std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    // Set of valid identifiers (P, Q, S)
    const std::unordered_set<std::string> validIdentifiers = {"P", "Q", "S"};

public:
    explicit Scanner(const std::string& source) : source(source) {}

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
    bool isAtEnd() const {
        return current >= source.length();
    }

    char advance() {
        return source[current++];
    }

    char peek() const {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    bool match(char expected) {
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;

        current++;
        return true;
    }

    void skipWhitespace() {
        while (std::isspace(source[current])) {
            if (source[current] == '\n') {
                line++;
            }
            current++;
        }
    }

    void scanToken() {
        skipWhitespace();

        char c = advance();
        if (isalpha(c)) {
            // Start of an identifier or keyword
            std::string identifier(1, c);
            while (isalpha(peek())) {
                identifier += advance();
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
            } else if (validIdentifiers.find(identifier) != validIdentifiers.end()) {
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

    void addToken(TokenType type, const std::string& lexeme) {
        tokens.push_back(Token(type, lexeme, nullptr, line));
    }
};

int main() {
    std::string source = "(P AND Q) IMPLIES OR A EQUIVALENT B";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    // Output the tokens
    for (const Token& token : tokens) {
        std::cout << "Token: " << token.lexeme << " (Type: ";
        switch (token.type) {
            case LEFT_PAREN: std::cout << "LEFT_PAREN"; break;
            case RIGHT_PAREN: std::cout << "RIGHT_PAREN"; break;
            case AND: std::cout << "AND"; break;
            case TRUE: std::cout << "TRUE"; break;
            case FALSE: std::cout << "FALSE"; break;
            case NOT: std::cout << "NOT"; break;
            case OR: std::cout << "OR"; break;
            case IMPLIES: std::cout << "IMPLIES"; break;
            case EQUIVALENT: std::cout << "EQUIVALENT"; break;
            case IDENTIFIER: std::cout << "IDENTIFIER"; break;
            case TOKENEOF: std::cout << "TOKENEOF"; break;
            case UNKNOWN: std::cout << "UNKNOWN"; break;
        }
        std::cout << ")\n";
    }

    return 0;
}
