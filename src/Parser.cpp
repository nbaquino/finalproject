#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "../include/Parser.h"

// Add this at the top level of the file (outside any function)
std::unordered_map<std::string, bool> AtomicSentence::booleanValues;

// AtomicSentence - Leaf node for atomic values
AtomicSentence::AtomicSentence(const std::string& identifier) : identifier(identifier) {}

bool AtomicSentence::evaluate() const {
    // Special handling for TRUE and FALSE literals
    if (identifier == "TRUE") return true;
    if (identifier == "FALSE") return false;

    // Existing logic for other identifiers
    auto it = booleanValues.find(identifier);
    if (it != booleanValues.end()) {
        return it->second;
    }
    std::cerr << "Warning: Undefined identifier '" << identifier << "' used in evaluation." << std::endl;
    return false;
}

void AtomicSentence::print(int indent) const {
    std::cout << std::string(indent, ' ') << "AtomicSentence (" << identifier << ")\n";
}

void AtomicSentence::setBooleanValue(const std::string& id, bool value) {
    booleanValues[id] = value;
}

// ComplexSentence - Node for binary operations
ComplexSentence::ComplexSentence(ParseTreeNode* left, TokenType connective, ParseTreeNode* right)
    : left(left), connective(connective), right(right) {}

ComplexSentence::~ComplexSentence() {
    delete left;
    delete right;
}

bool ComplexSentence::evaluate() const {
    if (!left || !right) {
        throw std::runtime_error("Invalid operands for connective.");
    }
    switch (connective) {
        case TokenType::AND:
            return left->evaluate() && right->evaluate();
        case TokenType::OR:
            return left->evaluate() || right->evaluate();
        case TokenType::IMPLIES:
            return !left->evaluate() || right->evaluate();
        case TokenType::EQUIVALENT:
            return left->evaluate() == right->evaluate();
        default:
            throw std::runtime_error("Unknown connective.");
    }
}

void ComplexSentence::print(int indent) const {
    std::cout << std::string(indent, ' ') << "ComplexSentence (" << static_cast<int>(connective) << ")\n";
    if (left) left->print(indent + 2);
    if (right) right->print(indent + 2);
}

// NegatedSentence - Node for unary NOT operation
NegatedSentence::NegatedSentence(ParseTreeNode* innerSentence) : innerSentence(innerSentence) {}

NegatedSentence::~NegatedSentence() {
    delete innerSentence;
}

bool NegatedSentence::evaluate() const {
    if (!innerSentence) {
        throw std::runtime_error("Invalid operand for NOT.");
    }
    return !innerSentence->evaluate();
}

void NegatedSentence::print(int indent) const {
    std::cout << std::string(indent, ' ') << "NegatedSentence\n";
    if (innerSentence) innerSentence->print(indent + 2);
}

// Parser class - Parsing logic
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

Node* Parser::parseSentence() {
    Node* left = parseTerm();
    while (current < tokens.size()) {
        const Token& token = tokens[current];
        if (token.type == TokenType::AND || token.type == TokenType::OR ||
            token.type == TokenType::IMPLIES || token.type == TokenType::EQUIVALENT) {
            TokenType connective = token.type;
            ++current;
            Node* right = parseTerm();
            if (!right) {
                throw std::runtime_error("Missing second operand after operator.");
            }
            Node* parent = new Node(token.lexeme);  // Create a new Node for the connective
            parent->children.push_back(left);
            parent->children.push_back(right);
            left = parent;
        } else {
            break;  // Or handle other token types as needed
        }
    }
    return left;
}

bool Parser::isConnective(TokenType type) {
    return type == TokenType::AND || type == TokenType::OR ||
           type == TokenType::IMPLIES || type == TokenType::EQUIVALENT;
}

Node* Parser::parseTerm() {
    if (current >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input.");
    }
    const Token& token = tokens[current];
    if (token.type == TokenType::IDENTIFIER ||
        token.lexeme == "TRUE" ||
        token.lexeme == "FALSE") {
        ++current;
        return new Node(token.lexeme);
    } else if (token.type == TokenType::NOT) {
        ++current;
        Node* inner = parseTerm();
        if (!inner) {
            throw std::runtime_error("NOT operator missing operand.");
        }
        Node* notNode = new Node("NOT");
        notNode->children.push_back(inner);
        return notNode;
    } else if (token.type == TokenType::LEFT_PAREN) {
        ++current;
        Node* expr = parseSentence();
        if (current >= tokens.size() || tokens[current].type != TokenType::RIGHT_PAREN) {
            throw std::runtime_error("Expected closing parenthesis.");
        }
        ++current;  // Consume ')'
        return expr;
    } else {
        throw std::runtime_error("Unexpected token '" + token.lexeme + "' encountered.");
    }
}
