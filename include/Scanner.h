#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h" // Include the Token class header
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>
#include <unordered_map>

/**
 * @brief Scanner class for tokenizing logical expressions
 *
 * This class processes input strings and breaks them down into tokens
 * for use in logical expression parsing. It handles keywords, operators,
 * and identifiers while tracking line numbers and detecting invalid tokens.
 */
class Scanner {
public:
    /**
     * @brief Constructs a Scanner object
     * @param input_string The logical expression to be tokenized
     */
    Scanner(const std::string& input_string);

    /**
     * @brief Processes the input string into tokens
     * @throws std::invalid_argument if an invalid token is encountered
     */
    void tokenize();

    /**
     * @brief Retrieves the list of tokens generated from the input
     * @return std::vector<Token> The list of tokens extracted from the input
     */
    std::vector<Token> getTokens() const;

    std::string getTokensAsString() const;

private:
    std::string input_string;                 // The original input string
    std::vector<Token> tokens;                // List of tokens extracted from the input string
    int line;                                 // Current line number

    /**
     * @brief Removes all whitespace characters from a string
     * @param str The input string to process
     * @return std::string The string with all spaces removed
     */
    std::string removeSpaces(const std::string& str);

    /**
     * @brief Locates the position of an invalid token in the input
     * @param cleaned_input The input string without spaces
     * @return size_t The position of the first invalid token
     */
    size_t findInvalidPosition(const std::string& cleaned_input);

    /**
     * @brief Creates a new Token object with the given parameters
     * @param type The TokenType of the token
     * @param lexeme The string value of the token
     * @param line The line number where the token appears
     * @return Token The created token object
     */
    Token createToken(TokenType type, const std::string& lexeme, int line);

    static const std::unordered_map<std::string, TokenType> keywords;

    /**
     * @brief Determines the TokenType for a given lexeme
     * @param lexeme The string to be classified
     * @return TokenType The type of the token (keyword, identifier, or operator)
     */
    TokenType getTokenType(const std::string& lexeme);
};

#endif // SCANNER_H
