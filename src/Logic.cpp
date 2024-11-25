#include <iostream>
#include <vector>
#include "../include/Evaluator.h"
#include "../include/Parser.h"
#include "../include/Evaluator.h"
#include <fstream>


static std::vector<std::string> read_input_file(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open input file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (!line.empty() && line[0] != '#') {
            lines.push_back(line);
        }
    }

    file.close();
    return lines;
}

// Add this function to process a single expression
static void process_expression(const std::string& input_string) {
    try {
        // Step 1: Tokenize the input string
        Scanner scanner(input_string);
        std::vector<Token> tokens = scanner.getTokens();
        std::cout << "Scanned Tokens: ";
        for (const auto& token : tokens) {
            std::cout << token.lexeme << " ";
        }
        std::cout << std::endl;

        // Step 2: Parse the tokens into a parse tree
        Parser parser(tokens);
        Node* parse_tree = parser.parseSentence();
        std::cout << "Parse Tree Structure: " << std::endl;
        print_tree(parse_tree);

        // Step 3 & 4: Generate and print truth table
        Evaluator evaluator(parse_tree);
        auto [table, finalColumns] = evaluator.generateTruthTable();

        std::cout << "Truth Table: \n";
        // Print header
        for (const auto& col : finalColumns) {
            std::cout << col << "\t";
        }
        std::cout << std::endl;

        // Print rows
        for (const auto& row : table) {
            const auto& values = row.first;
            const auto& results = row.second;
            for (const auto& var : finalColumns) {
                if (values.find(var) != values.end()) {
                    std::cout << (values.at(var) ? "T" : "F") << "\t";
                } else {
                    std::cout << (results.at(var) ? "T" : "F") << "\t";
                }
            }
            std::cout << std::endl;
        }

        // Clean up parse tree
        delete parse_tree;
    } catch (const std::exception& e) {
        std::cout << "Error processing '" << input_string << "': " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: " << argv[0] << " [input_file]" << std::endl;
        return 1;
    }

    if (argc == 2) {
        // File input mode
        try {
            std::vector<std::string> inputs = read_input_file(argv[1]);
            for (const auto& input_string : inputs) {
                std::cout << "\nProcessing: " << input_string << std::endl;
                process_expression(input_string);
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            return 1;
        }
    } else {
        // Interactive mode
        std::cout << "Propositional Logic Evaluator" << std::endl;
        std::cout << "Enter 'exit' or 'quit' to terminate the program." << std::endl;

        while (true) {
            std::cout << "\nEnter a propositional logic statement: ";
            std::string input_string;
            std::getline(std::cin, input_string);

            if (input_string == "exit" || input_string == "quit") {
                std::cout << "Program terminated." << std::endl;
                break;
            }

            process_expression(input_string);
        }
    }

    return 0;
}
