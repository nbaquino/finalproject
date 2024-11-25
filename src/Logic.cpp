#include <iostream>
#include <vector>
#include "Scanner.h"
#include "Parser.h"
#include "Evaluator.h"

// Assuming Node is now used instead of ParseTreeNode
void print_tree(Node* node, int level = 0) {
    if (node == nullptr) return;
    std::cout << std::string(level * 2, ' ') << "Node: ";
    // Assuming Node has a method to print itself or you can directly access its value
    std::cout << node->value << std::endl;  // Simplified for demonstration
    for (auto child : node->children) {
        print_tree(child, level + 1);
    }
}

int main() {
    std::cout << "This is a Propositional Logic Evaluator." << std::endl;
    std::cout << "Enter 'exit' or 'quit' to terminate the program." << std::endl;

    while (true) {
        std::cout << "\nEnter a propositional logic statement: ";
        std::string input_string;
        std::getline(std::cin, input_string);

        // Exit condition
        if (input_string == "exit" || input_string == "quit") {
            std::cout << "Program terminated." << std::endl;
            break;
        }

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
            Node* parse_tree = parser.parseSentence();  // Updated to use Node*
            std::cout << "Parse Tree Structure: " << std::endl;
            print_tree(parse_tree);


            // Step 3: Generate the truth table
            std::vector<std::string> variables = {"P", "Q"};
            Evaluator evaluator(parse_tree);
            auto [table, finalColumns] = evaluator.generateTruthTable(variables);  

            // Step 4: Print the truth table
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
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
