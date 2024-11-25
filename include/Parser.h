#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "Scanner.h"
#include "Node.h"

/**
 * @brief Abstract base class for parse tree nodes
 *
 * Defines the interface for all nodes in the parse tree,
 * providing virtual methods for evaluation and printing.
 */
class ParseTreeNode {
public:
    virtual ~ParseTreeNode() = default;

    /**
     * @brief Prints the node's contents with specified indentation
     * @param indent Number of spaces to indent the output
     */
    virtual void print(int indent = 0) const = 0;

    /**
     * @brief Evaluates the logical expression represented by this node
     * @return bool The result of evaluating the expression
     */
    virtual bool evaluate() const = 0;
};

/**
 * @brief Represents an atomic (leaf) node in the parse tree
 *
 * Handles individual boolean variables and their values.
 */
class AtomicSentence : public ParseTreeNode {
public:
    /**
     * @brief Constructs an atomic sentence with an identifier
     * @param identifier The name of the boolean variable
     */
    explicit AtomicSentence(const std::string& identifier);

    bool evaluate() const override;
    void print(int indent = 0) const override;

    /**
     * @brief Sets the boolean value for a variable
     * @param id The identifier of the variable
     * @param value The boolean value to assign
     */
    static void setBooleanValue(const std::string& id, bool value);

private:
    std::string identifier;
    static std::unordered_map<std::string, bool> booleanValues;
};

/**
 * @brief Represents a complex sentence with binary logical operations
 *
 * Handles binary operations (AND, OR, IMPLIES, etc.) between two sub-expressions.
 */
class ComplexSentence : public ParseTreeNode {
public:
    /**
     * @brief Constructs a complex sentence with two operands and an operator
     * @param left Left operand of the expression
     * @param connective The logical operator
     * @param right Right operand of the expression
     */
    ComplexSentence(ParseTreeNode* left, TokenType connective, ParseTreeNode* right);
    ~ComplexSentence() override;

    bool evaluate() const override;
    void print(int indent = 0) const override;

private:
    ParseTreeNode* left;
    TokenType connective;
    ParseTreeNode* right;
};

/**
 * @brief Represents a negated logical expression
 *
 * Handles the logical NOT operation on a sub-expression.
 */
class NegatedSentence : public ParseTreeNode {
public:
    /**
     * @brief Constructs a negated sentence
     * @param innerSentence The expression to be negated
     */
    explicit NegatedSentence(ParseTreeNode* innerSentence);
    ~NegatedSentence() override;

    bool evaluate() const override;
    void print(int indent = 0) const override;

private:
    ParseTreeNode* innerSentence;
};

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

private:
    const std::vector<Token>& tokens;
    size_t current;

    /**
     * @brief Checks if a token type represents a logical connective
     * @param type The token type to check
     * @return bool True if the token is a logical connective
     */
    bool isConnective(TokenType type);

    /**
     * @brief Parses a term (atomic sentence or parenthesized expression)
     * @return Node* Root node of the parsed term
     * @throws std::runtime_error if parsing fails
     */
    Node* parseTerm();
};

#endif // PARSER_H
