#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>  // For isalpha(), isspace()

/**
 * Enumaration representing the different token types in the language.
 */
enum class TokenType {
    O_Paren, C_Paren,
    Identifier, True, False,
    Not, And, Or, Implies, Equivalent,
    TokenEOF,
    Unknown
};

/**
 * Class representing a token with its type and lexeme.
 */
class Token {
public:
    const TokenType type; /**< Type of the token. */
    const std::string lexeme; /**< Lexeme of the token. */

    /**
     * Constructor for Token class.
     * @param type Type of the token.
     * @param lexeme Lexeme of the token.
     */
    Token(TokenType type, const std::string& lexeme)
        : type(type), lexeme(lexeme) {};
};

/**
 * Class responsible for scanning the source code and generating a sequence of tokens.
 */
class Scanner {
private:
    const std::string source; /**< Source code to be scanned. */
    std::vector<Token> tokens; /**< Vector to store the generated tokens. */
    size_t current = 0; /**< Current position in the source code. */
    size_t start = 0; /**< Starting position of the current token being scanned. */

public:
    /**
     * Constructor for the Scanner class.
     * @param source The source code to be scanned.
     */
    Scanner(const std::string& source) : source(source) {}

    /**
     * Scans the next token from the source code and adds it to the list of tokens.
     */
    void scanToken() {
        skipWhiteSpace();

        /**
         * Unordered map used to associate strings to their corresponding token types
         */
        char currentChar = source[current];
        std::unordered_map<std::string, TokenType> tokenMap = {
            {"(", TokenType::O_Paren},
            {")", TokenType::C_Paren},
            {"AND", TokenType::And},
            {"NOT", TokenType::Not},
            {"OR", TokenType::Or},
            {"IMPLIES", TokenType::Implies},
            {"EQUIVALENT", TokenType::Equivalent},
        };

        if (currentChar == '(') {
            addToken(TokenType::O_Paren, std::string(1, currentChar));
            current++;
            return;
        }

        std::string lexeme(1, currentChar);
        while (std::isalpha(source[current + 1])) {
            lexeme += source[++current];
        }

        auto iterator = tokenMap.find(lexeme);
        if (iterator != tokenMap.end()) {
            addToken(iterator->second, lexeme);
        } else if (lexeme == "P" || lexeme == "Q") {
            addToken(TokenType::Identifier, lexeme);
        } else {
            addToken(TokenType::Unknown, lexeme);
        }

        current++;
    }

    /**
     * Adds a token to the list of tokens with the given type and lexeme.
     * @param type Type of the token to be added.
     * @param lexeme Lexeme associated with the token.
     */
    void addToken(TokenType type, const std::string& lexeme) {
        tokens.emplace_back(type, lexeme);
    }

    const std::vector<Token>& getTokens() const {
        return tokens;
    }

    /**
     * Skips whitespaces in the source code.
     */
    void skipWhiteSpace() {
        while (std::isspace(source[current])) {
            current++;
        }
    }

    /**
     * Tokenizes the entire source code.
     * @return Vector containing the generated tokens.
     */
    std::vector<Token> tokenize() {
        while (current < source.length()) {
            start = current;
            scanToken();
        }

        addToken(TokenType::TokenEOF, "");

        return tokens;
    }
};

/**
 * Main function to test the Scanner class.
 */
int main() {
    // Example source code to tokenize
    std::string source = "(P AND Q) OR S";

    // Create a Scanner object
    Scanner scanner(source);

    // Tokenize the source code
    std::vector<Token> tokens = scanner.tokenize();

    // Output the tokens and their types
    for (const Token& token : tokens) {
        std::cout << "Token: " << token.lexeme << " (Type: ";

        // Print the type of the token
        switch (token.type) {
            case TokenType::O_Paren: std::cout << "O_Paren"; break;
            case TokenType::C_Paren: std::cout << "C_Paren"; break;
            case TokenType::Identifier: std::cout << "Identifier"; break;
            case TokenType::True: std::cout << "True"; break;
            case TokenType::False: std::cout << "False"; break;
            case TokenType::Not: std::cout << "Not"; break;
            case TokenType::And: std::cout << "And"; break;
            case TokenType::Or: std::cout << "Or"; break;
            case TokenType::Implies: std::cout << "Implies"; break;
            case TokenType::Equivalent: std::cout << "Equivalent"; break;
            case TokenType::TokenEOF: std::cout << "TokenEOF"; break;
            case TokenType::Unknown: std::cout << "Unknown"; break;
        }
        std::cout << ")\n";
    }

    return 0;
}
