#include <iostream>
#include <vector>
#include "../include/Scanner.h"

//Class for Sentence


//Class for Atomic Sentence


//Class for Complex Sentence


//Class for Negated Sentence


class Parser {
private:
    const std::vector<Token>& tokens;
    int current = 0;

public:
    /*
     * Constructor for the Parser class
     * @param tokens
     * The vector of tokens to be parsed
     */
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    /*
     * Checks if the current token matches the given type and advances if it does
     * @param type
     * The token type to match against
     * @return
     * True if the current token matches the given type (and advances), false otherwise
     */
    bool match(TokenType type) {
        if (current >= tokens.size()) return false;  // This check is important to prevent accessing tokens out of bounds
        if (tokens[current].type == type) {
            current++;
            return true;
        }
        return false;
    }
};
