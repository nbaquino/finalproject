#include <iostream>
#include <vector>
#include "../include/Evaluator.h"
#include "../include/Parser.h"
#include "../include/Evaluator.h"
#include <fstream>
#include <iomanip>


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

        // Step 2: Parse the tokens into a parse tree
        Parser parser(tokens);
        Node* parse_tree = parser.parseSentence();

        // Step 3 & 4: Generate and print truth table
        Evaluator evaluator(parse_tree);
        auto [table, finalColumns] = evaluator.generateTruthTable();

        std::cout << "Truth Table:\n";

        // Calculate column widths
        std::vector<size_t> columnWidths;
        for (const auto& col : finalColumns) {
            columnWidths.push_back(std::max(col.length(), size_t(5)) + 2); // minimum width of 5 plus padding
        }

        // Print header with proper alignment
        for (size_t i = 0; i < finalColumns.size(); ++i) {
            std::cout << std::left << std::setw(columnWidths[i]) << finalColumns[i] << "|";
        }
        std::cout << "\n";

        // Print separator line
        for (const auto& width : columnWidths) {
            std::cout << std::string(width, '-') << "+";
        }
        std::cout << "\n";

        // Print rows with proper alignment
        for (const auto& row : table) {
            const auto& values = row.first;
            const auto& results = row.second;

            for (size_t i = 0; i < finalColumns.size(); ++i) {
                const auto& col = finalColumns[i];
                bool value;
                if (values.find(col) != values.end()) {
                    value = values.at(col);
                } else {
                    value = results.at(col);
                }
                std::cout << std::left << std::setw(columnWidths[i]) << (value ? "T" : "F") << "|";
            }
            std::cout << "\n";
        }

        // Clean up parse tree
        delete parse_tree;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
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
        std::cout << "CMSC 124 - Propositional Logic Evaluator" << std::endl;
        std::cout << "Enter 'exit' to terminate the program." << std::endl;

        while (true) {
            std::cout << "\nEnter a propositional logic: ";
            std::string input_string;
            std::getline(std::cin, input_string);

            if (input_string == "exit") {
                std::cout << "Program terminated." << std::endl;
                break;
            }

            process_expression(input_string);
        }
    }

    return 0;
}
