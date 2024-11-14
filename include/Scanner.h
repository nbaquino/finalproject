#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

/**
 * External declaration for a set of valid identifiers used in the scanner.
 */
extern std::unordered_set<std::string> validIds;

/**
 * Class responsible for scanning the source code and generating a sequence of tokens.
 */
class Scanner {
public:
    /**
     * Constructor that initializes the scanner with a source string.
     * @param source The source code to be scanned.
     */
    explicit Scanner(const std::string& source);

    /**
     * Scans the entire source string and returns a vector of tokens.
     * @return Vector of tokens generated from the source.
     */
    std::vector<Token> scanTokens();

    /**
     * Tokenizes the entire source code, similar to scanTokens but may be used differently.
     * @return Vector of tokens generated from the source.
     */
    std::vector<Token> tokenize();

private:
    const std::string source; /**< Source string to be tokenized. */
    std::vector<Token> tokens; /**< List of tokens generated from the source. */
    int start = 0; /**< Start index of the current token being scanned. */
    int current = 0; /**< Current index in the source string. */
    int line = 1; /**< Current line number in the source string. */

    bool isAtEnd() const; /**< Returns true if the end of the source has been reached. */
    char advance(); /**< Advances the current index and returns the character at the new index. */
    char peek() const; /**< Returns the current character without advancing the index. */
    bool match(char expected); /**< Checks and consumes the current character if it matches the expected character. */
    void skipWhitespace(); /**< Skips whitespace and handles new lines. */
    void scanToken(); /**< Scans and adds the next token from the source. */
    void addToken(TokenType type, const std::string& lexeme); /**< Adds a token to the list with the specified type and lexeme. */
};

#endif // SCANNER_H
