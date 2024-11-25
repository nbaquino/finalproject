#include "Scanner.h"
#include "Token.h"  // Include the Token header to use the Token class

// Constructor that accepts the input string to be tokenized
Scanner::Scanner(const std::string& input_string) : input_string(input_string), line(1) {
    tokenize();
}

// Tokenizes the input string and checks for invalid tokens
void Scanner::tokenize() {
    std::regex token_regex("(TRUE|FALSE|P|Q|S|NOT|AND|OR|IMPLIES|EQUIVALENT|\\(|\\))");
    std::sregex_token_iterator iter(input_string.begin(), input_string.end(), token_regex);
    std::sregex_token_iterator end;

    while (iter != end) {
        std::string token_str = iter->str();
        TokenType type = TokenType::UNKNOWN;

        // Match token type based on the lexeme
        if (token_str == "TRUE") {
            type = TokenType::TRUE;
        } else if (token_str == "FALSE") {
            type = TokenType::FALSE;
        } else if (token_str == "P" || token_str == "Q" || token_str == "S") {
            type = TokenType::IDENTIFIER;
        } else if (token_str == "NOT") {
            type = TokenType::NOT;
        } else if (token_str == "AND") {
            type = TokenType::AND;
        } else if (token_str == "OR") {
            type = TokenType::OR;
        } else if (token_str == "IMPLIES") {
            type = TokenType::IMPLIES;
        } else if (token_str == "EQUIVALENT") {
            type = TokenType::EQUIVALENT;
        } else if (token_str == "(") {
            type = TokenType::LEFT_PAREN;
        } else if (token_str == ")") {
            type = TokenType::RIGHT_PAREN;
        }

        // Create a Token object and add it to the tokens list
        tokens.push_back(createToken(type, token_str, line));

        ++iter;
    }

    // Remove spaces to compare cleaned input
    std::string cleaned_input;
    for (const auto& token : tokens) {
        cleaned_input += token.lexeme;
    }

    // Check for invalid tokens
    if (cleaned_input != removeSpaces(input_string)) {
        size_t invalid_index = findInvalidPosition(cleaned_input);
        throw std::invalid_argument("There is a typographical error at position " +
                                    std::to_string(invalid_index) +
                                    ". Invalid token: '" + input_string.substr(invalid_index, 1) + "'");
    }
}

// Returns the tokens extracted from the input string
std::vector<Token> Scanner::getTokens() const {
    return tokens;
}

// Helper method to remove spaces from a string
std::string Scanner::removeSpaces(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (!std::isspace(ch)) {
            result += ch;
        }
    }
    return result;
}

// Helper method to find the position of the invalid token
size_t Scanner::findInvalidPosition(const std::string& cleaned_input) {
    size_t i = 0;
    for ( ; i < cleaned_input.size() && i < input_string.size(); ++i) {
        if (input_string[i] != cleaned_input[i]) {
            return i;
        }
    }
    return i;
}

// Helper method to create a Token from a matched string
Token Scanner::createToken(TokenType type, const std::string& lexeme, int line) {
    return Token(type, lexeme, nullptr, line); // For simplicity, we pass nullptr as the literal value
}
