#ifndef SCANNER_H
#define SCANNER_H

#include "../include/Token.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

extern std::unordered_set<std::string> validIds;

class Scanner {
public:
    explicit Scanner(const std::string& source);
    std::vector<Token> scanTokens();

private:
    const std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd() const;
    char move();
    char peek() const;
    bool match(char expected);
    char advance();
    void skipWhitespace();
    void scanToken();
    void addToken(TokenType type, const std::string& lexeme);
};

#endif // SCANNER_H
