#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>

// Token types for the lexical analyzer
enum TokenType {
    LEFT_PAREN,
    RIGHT_PAREN,
    IDENTIFIER,
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

#endif // TOKEN_H
