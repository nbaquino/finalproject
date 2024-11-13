#include <vector>
#include <memory>
#include <stdexcept>
#include "Scanner.cpp"  // Assuming Scanner.h exists with Token and TokenType definitions

// Forward declarations for our AST nodes
class Expr;
class BinaryExpr;
class UnaryExpr;
class LiteralExpr;

// Visitor pattern for evaluating expressions
class ExprVisitor {
public:
    virtual void visitBinaryExpr(const BinaryExpr& expr) = 0;
    virtual void visitUnaryExpr(const UnaryExpr& expr) = 0;
    virtual void visitLiteralExpr(const LiteralExpr& expr) = 0;
    virtual ~ExprVisitor() = default;
};

// Base Expression class
class Expr {
public:
    virtual void accept(ExprVisitor& visitor) const = 0;
    virtual ~Expr() = default;
};

// Binary Expression (AND, OR, IMPLIES, EQUIVALENT)
class BinaryExpr : public Expr {
public:
    BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(left), op(op), right(right) {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visitBinaryExpr(*this);
    }

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

// Unary Expression (NOT)
class UnaryExpr : public Expr {
public:
    UnaryExpr(Token op, std::shared_ptr<Expr> right)
        : op(op), right(right) {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visitUnaryExpr(*this);
    }

    const Token op;
    const std::shared_ptr<Expr> right;
};

// Literal Expression (TRUE, FALSE, IDENTIFIER)
class LiteralExpr : public Expr {
public:
    explicit LiteralExpr(Token value) : value(value) {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visitLiteralExpr(*this);
    }

    const Token value;
};

class Parser {
private:
    const std::vector<Token>& tokens;
    int current = 0;

public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    std::shared_ptr<Expr> parse() {
        try {
            return expression();
        } catch (const std::runtime_error& error) {
            // Handle parsing errors
            return nullptr;
        }
    }

private:
    std::shared_ptr<Expr> expression() {
        return equivalent();
    }

    std::shared_ptr<Expr> equivalent() {
        auto expr = implies();

        while (match(EQUIVALENT)) {
            Token op = previous();
            auto right = implies();
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<Expr> implies() {
        auto expr = or_();

        while (match(IMPLIES)) {
            Token op = previous();
            auto right = or_();
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<Expr> or_() {
        auto expr = and_();

        while (match(OR)) {
            Token op = previous();
            auto right = and_();
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<Expr> and_() {
        auto expr = unary();

        while (match(AND)) {
            Token op = previous();
            auto right = unary();
            expr = std::make_shared<BinaryExpr>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<Expr> unary() {
        if (match(NOT)) {
            Token op = previous();
            auto right = unary();
            return std::make_shared<UnaryExpr>(op, right);
        }

        return primary();
    }

    std::shared_ptr<Expr> primary() {
        if (match(TRUE) || match(FALSE) || match(IDENTIFIER)) {
            return std::make_shared<LiteralExpr>(previous());
        }

        if (match(LEFT_PAREN)) {
            auto expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression.");
            return expr;
        }

        throw std::runtime_error("Expect expression.");
    }

    bool match(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    bool check(TokenType type) const {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    Token advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd() const {
        return peek().type == TOKENEOF;
    }

    Token peek() const {
        return tokens[current];
    }

    Token previous() const {
        return tokens[current - 1];
    }

    Token consume(TokenType type, const std::string& message) {
        if (check(type)) return advance();
        throw std::runtime_error(message);
    }
};
