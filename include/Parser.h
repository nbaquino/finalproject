#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "Scanner.h"  // Assuming Token and TokenType are defined here
#include "Node.h"  // Include the Node header

// ParseTreeNode - Abstract base class for nodes
class ParseTreeNode {
public:
    virtual ~ParseTreeNode() = default;

    // Virtual methods to be overridden by derived classes
    virtual void print(int indent = 0) const = 0;
    virtual bool evaluate() const = 0;
};

// AtomicSentence - Leaf node for atomic values
class AtomicSentence : public ParseTreeNode {
public:
    explicit AtomicSentence(const std::string& identifier);

    bool evaluate() const override;
    void print(int indent = 0) const override;

    static void setBooleanValue(const std::string& id, bool value);

private:
    std::string identifier;
    static std::unordered_map<std::string, bool> booleanValues;
};

// ComplexSentence - Node for binary operations
class ComplexSentence : public ParseTreeNode {
public:
    ComplexSentence(ParseTreeNode* left, TokenType connective, ParseTreeNode* right);
    ~ComplexSentence() override;

    bool evaluate() const override;
    void print(int indent = 0) const override;

private:
    ParseTreeNode* left;
    TokenType connective;
    ParseTreeNode* right;
};

// NegatedSentence - Node for unary NOT operation
class NegatedSentence : public ParseTreeNode {
public:
    explicit NegatedSentence(ParseTreeNode* innerSentence);
    ~NegatedSentence() override;

    bool evaluate() const override;
    void print(int indent = 0) const override;

private:
    ParseTreeNode* innerSentence;
};

// Parser class - Parsing logic
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    // Method to parse a sentence (logic expression)
    Node* parseSentence();

private:
    const std::vector<Token>& tokens;
    size_t current;

    bool isConnective(TokenType type);
    Node* parseTerm();
};

#endif // PARSER_H
