#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "../include/Parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

Node* Parser::parseSentence() {
    Node* expr = parseTerm();

    while (current < tokens.size()) {
        if (tokens[current].type == TokenType::LEFT_PAREN) {
            if (current + 1 < tokens.size() && isConnective(tokens[current + 1].type)) {
                delete expr;
                throw std::runtime_error("Binary operator '" + tokens[current + 1].lexeme +
                                       "' must have both left and right operands");
            }
            delete expr;
            throw std::runtime_error("Unexpected '(' after expression");
        }

        if (tokens[current].type == TokenType::NOT) {
            delete expr;
            throw std::runtime_error("NOT operator can only appear before its operand");
        }

        if (isConnective(tokens[current].type)) {
            Token op = tokens[current];
            validateBinaryOperator(op);
            ++current;

            Node* right = parseTerm();
            if (!right) {
                delete expr;
                throw std::runtime_error("Missing right operand after '" + op.lexeme + "'");
            }

            Node* newExpr = new Node(op.lexeme);
            newExpr->children.push_back(expr);
            newExpr->children.push_back(right);
            expr = newExpr;
        } else {
            break;
        }
    }

    if (current < tokens.size() && tokens[current].type != TokenType::RIGHT_PAREN) {
        delete expr;
        throw std::runtime_error("Unexpected tokens after valid expression");
    }

    return expr;
}

Node* Parser::parseTerm() {
    if (current >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }

    const Token& token = tokens[current];
    ++current;

    switch (token.type) {
        case TokenType::IDENTIFIER:
        case TokenType::TRUE:
        case TokenType::FALSE:
            return new Node(token.lexeme);

        case TokenType::NOT: {
            Node* operand = parseTerm();
            if (!operand) {
                throw std::runtime_error("Missing operand after NOT");
            }
            Node* notNode = new Node("NOT");
            notNode->children.push_back(operand);
            return notNode;
        }

        case TokenType::LEFT_PAREN: {
            if (current < tokens.size() && isConnective(tokens[current].type)) {
                throw std::runtime_error("Binary operator cannot appear directly after '('");
            }
            Node* expr = parseSentence();
            if (current >= tokens.size() || tokens[current].type != TokenType::RIGHT_PAREN) {
                delete expr;
                throw std::runtime_error("Missing closing parenthesis");
            }
            ++current;
            return expr;
        }

        case TokenType::AND:
        case TokenType::OR:
        case TokenType::IMPLIES:
        case TokenType::EQUIVALENT:
            throw std::runtime_error("Binary operator '" + token.lexeme + "' must have both left and right operands");

        default:
            throw std::runtime_error("Unexpected token '" + token.lexeme + "'");
    }
}

void Parser::validateBinaryOperator(const Token& op) {
    if (current > 0 && tokens[current - 1].type == TokenType::LEFT_PAREN) {
        throw std::runtime_error("Binary operator '" + op.lexeme +
                               "' inside parentheses must have a left operand");
    }
}

bool Parser::isConnective(TokenType type) {
    return type == TokenType::AND || type == TokenType::OR ||
           type == TokenType::IMPLIES || type == TokenType::EQUIVALENT;
}
