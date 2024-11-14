#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>


/**
 * Enumeration representing the different token types in the language.
 */
enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN,
    IDENTIFIER, TRUE, FALSE,
    NOT, AND, OR, IMPLIES, EQUIVALENT,
    TOKENEOF,
    UNKNOWN
};

/**
 * Class representing a token with its type, lexeme, and optional literal value.
 */
class Token {
public:
    const TokenType type; /**< Type of the token. */    
    const std::string lexeme; /**< Lexeme of the token. */
    const std::shared_ptr<void> literal; /**< Optional literal value of the token. */
    int line; /**< Line number where the token was found. */

    /**
     * Constructor for Token class.
     * @param type Type of the token.
     * @param lexeme Lexeme of the token.
     * @param literal Optional literal value of the token.
     * @param line Line number where the token was found.
     */
    Token(TokenType type, const std::string& lexeme, std::shared_ptr<void> literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}

    // Static function to convert TokenType to string
    static std::string tokenTypeToString(TokenType type) {
        switch (type) {
            case TokenType::LEFT_PAREN: return "LEFT_PAREN";
            case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::TRUE: return "TRUE";
            case TokenType::FALSE: return "FALSE";
            case TokenType::NOT: return "NOT";
            case TokenType::AND: return "AND";
            case TokenType::OR: return "OR";
            case TokenType::IMPLIES: return "IMPLIES";
            case TokenType::EQUIVALENT: return "EQUIVALENT";
            case TokenType::TOKENEOF: return "TOKENEOF";
            case TokenType::UNKNOWN: return "UNKNOWN";
            default: return "UNDEFINED";
        }
    }
};

#endif // TOKEN_H
