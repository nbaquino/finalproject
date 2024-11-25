#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "../include/Parser.h"

/**
 * @brief Represents an atomic (leaf) node in the parse tree for boolean expressions
 */
class AtomicSentence {
public:
    static std::unordered_map<std::string, bool> booleanValues;
    AtomicSentence(const std::string& identifier) : identifier(identifier) {}

    bool evaluate() const {
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

    void print(int indent) const {
        std::cout << std::string(indent, ' ') << "AtomicSentence (" << identifier << ")\n";
    }

    void setBooleanValue(const std::string& id, bool value) {
        booleanValues[id] = value;
    }

private:
    std::string identifier;
};

/**
 * @brief Static map to store boolean values for atomic sentences
 */
std::unordered_map<std::string, bool> AtomicSentence::booleanValues;

/**
 * @brief Constructs an atomic sentence with the given identifier
 * @param identifier The name or value of the atomic sentence
 */
AtomicSentence::AtomicSentence(const std::string& identifier) : identifier(identifier) {}

/**
 * @brief Evaluates the atomic sentence to its boolean value
 * @return bool The truth value of the atomic sentence
 */
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

/**
 * @brief Prints the atomic sentence with the specified indentation
 * @param indent Number of spaces to indent the output
 */
void AtomicSentence::print(int indent) const {
    std::cout << std::string(indent, ' ') << "AtomicSentence (" << identifier << ")\n";
}

/**
 * @brief Sets the boolean value for an atomic sentence identifier
 * @param id The identifier to set
 * @param value The boolean value to assign
 */
void AtomicSentence::setBooleanValue(const std::string& id, bool value) {
    booleanValues[id] = value;
}

/**
 * @brief Represents a complex sentence with binary logical operations
 */
class ComplexSentence {
public:
    ComplexSentence(ParseTreeNode* left, TokenType connective, ParseTreeNode* right)
        : left(left), connective(connective), right(right) {}

    ~ComplexSentence() {
        delete left;
        delete right;
    }

    bool evaluate() const {
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

    void print(int indent) const {
        std::cout << std::string(indent, ' ') << "ComplexSentence (" << static_cast<int>(connective) << ")\n";
        if (left) left->print(indent + 2);
        if (right) right->print(indent + 2);
    }

private:
    ParseTreeNode* left;
    TokenType connective;
    ParseTreeNode* right;
};

/**
 * @brief Constructs a complex sentence with left and right operands and a connective
 * @param left The left operand
 * @param connective The logical operator
 * @param right The right operand
 */
ComplexSentence::ComplexSentence(ParseTreeNode* left, TokenType connective, ParseTreeNode* right)
    : left(left), connective(connective), right(right) {}

/**
 * @brief Evaluates the complex sentence according to its logical operator
 * @return bool The result of the logical operation
 * @throws std::runtime_error if operands are invalid or connective is unknown
 */
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

/**
 * @brief Prints the complex sentence with the specified indentation
 * @param indent Number of spaces to indent the output
 */
void ComplexSentence::print(int indent) const {
    std::cout << std::string(indent, ' ') << "ComplexSentence (" << static_cast<int>(connective) << ")\n";
    if (left) left->print(indent + 2);
    if (right) right->print(indent + 2);
}

/**
 * @brief Represents a negated sentence (NOT operation)
 */
class NegatedSentence {
public:
    NegatedSentence(ParseTreeNode* innerSentence) : innerSentence(innerSentence) {}

    ~NegatedSentence() {
        delete innerSentence;
    }

    bool evaluate() const {
        if (!innerSentence) {
            throw std::runtime_error("Invalid operand for NOT.");
        }
        return !innerSentence->evaluate();
    }

    void print(int indent) const {
        std::cout << std::string(indent, ' ') << "NegatedSentence\n";
        if (innerSentence) innerSentence->print(indent + 2);
    }

private:
    ParseTreeNode* innerSentence;
};

/**
 * @brief Constructs a negated sentence
 * @param innerSentence The sentence to be negated
 */
NegatedSentence::NegatedSentence(ParseTreeNode* innerSentence) : innerSentence(innerSentence) {}

/**
 * @brief Evaluates the negated sentence
 * @return bool The logical NOT of the inner sentence
 * @throws std::runtime_error if the inner sentence is invalid
 */
bool NegatedSentence::evaluate() const {
    if (!innerSentence) {
        throw std::runtime_error("Invalid operand for NOT.");
    }
    return !innerSentence->evaluate();
}

/**
 * @brief Prints the negated sentence with the specified indentation
 * @param indent Number of spaces to indent the output
 */
void NegatedSentence::print(int indent) const {
    std::cout << std::string(indent, ' ') << "NegatedSentence\n";
    if (innerSentence) innerSentence->print(indent + 2);
}

/**
 * @brief Parser class for building parse trees from tokens
 */
class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

    Node* parseSentence() {
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

    bool isConnective(TokenType type) {
        return type == TokenType::AND || type == TokenType::OR ||
               type == TokenType::IMPLIES || type == TokenType::EQUIVALENT;
    }

    Node* parseTerm() {
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

private:
    std::vector<Token> tokens;
    int current;
};

/**
 * @brief Constructs a parser with a vector of tokens
 * @param tokens The tokens to be parsed
 */
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

/**
 * @brief Parses a complete sentence, handling binary operations
 * @return Node* Root node of the parsed sentence
 * @throws std::runtime_error if parsing fails
 */
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

/**
 * @brief Checks if a token type is a logical connective
 * @param type The token type to check
 * @return bool True if the token is a logical connective
 */
bool Parser::isConnective(TokenType type) {
    return type == TokenType::AND || type == TokenType::OR ||
           type == TokenType::IMPLIES || type == TokenType::EQUIVALENT;
}

/**
 * @brief Parses a term (atomic sentence, negation, or parenthesized expression)
 * @return Node* Root node of the parsed term
 * @throws std::runtime_error if parsing fails
 */
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
