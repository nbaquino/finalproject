#include <iostream>
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    int current = 0;

public:
    Parser(std::vector<Token> tokens) : tokens(tokens) {}
};
