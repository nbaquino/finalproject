#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stdexcept>
#include "Scanner.h"
#include "Node.h"

/**
 * @brief Parser for logical expressions
 *
 * Converts a sequence of tokens into a parse tree that can be
 * evaluated to determine the truth value of the expression.
 */
class Parser {
public:
    /**
     * @brief Constructs a parser with a list of tokens
     * @param tokens Vector of tokens to be parsed
     */
    explicit Parser(const std::vector<Token>& tokens);

    /**
     * @brief Parses a complete logical expression
     * @return Node* Root node of the parsed expression tree
     * @throws std::runtime_error if parsing fails
     */
    Node* parseSentence();

    /**
     * @brief Checks if there are remaining tokens to parse
     * @return bool True if there are more tokens
     */
    bool hasMoreTokens() const { return current < tokens.size(); }

private:
    const std::vector<Token>& tokens;
    size_t current;

    void validateBinaryOperator(const Token& op);
    bool isConnective(TokenType type);
    Node* parseTerm();
};

#endif // PARSER_H
