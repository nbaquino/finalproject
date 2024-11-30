#include <iostream>
#include <algorithm>
#include <cmath>
#include "../include/Evaluator.h"

/**
 * @brief Constructs an Evaluator with a given syntax tree root
 * @param root Pointer to the root node of the syntax tree
 */
Evaluator::Evaluator(Node* root) : root(root) {}

/**
 * @brief Evaluates a logical expression while tracking intermediate results
 * @param node Current node in the syntax tree
 * @param values Map of variable assignments
 * @param subResults Map to store intermediate evaluation results
 * @return Boolean result of the evaluation
 */
bool Evaluator::evaluateWithTracking(Node* node, const std::unordered_map<std::string, bool>& values,
                                     std::unordered_map<std::string, bool>& subResults) {
    if (!node) {
        throw std::runtime_error("Invalid expression: Empty node encountered");
    }

    // Handle constants
    if (node->value == "TRUE") {
        subResults[node->value] = true;  // Store the result for TRUE
        return true;
    }
    if (node->value == "FALSE") {
        subResults[node->value] = false;  // Store the result for FALSE
        return false;
    }

    // Handle variables
    if (values.count(node->value)) {
        subResults[node->value] = values.at(node->value);  // Store the variable value
        return values.at(node->value);
    }

    // Get result first
    bool result;

    // Handle NOT operator
    if (node->value == "NOT") {
        result = !evaluateWithTracking(node->children[0], values, subResults);
        std::string expr = "NOT " + nodeToString(node->children[0]);
        subResults[expr] = result;
        return result;
    }

    // Handle binary operators
    bool leftResult = evaluateWithTracking(node->children[0], values, subResults);
    bool rightResult = evaluateWithTracking(node->children[1], values, subResults);

    if (node->value == "AND") result = leftResult && rightResult;
    else if (node->value == "OR") result = leftResult || rightResult;
    else if (node->value == "IMPLIES") result = !leftResult || rightResult;
    else if (node->value == "EQUIVALENT") result = leftResult == rightResult;
    else throw std::runtime_error("Unknown operator: '" + node->value + "'");

    // Store the result with proper formatting
    std::string expr = nodeToString(node);
    if (!subResults.count(expr)) {
        subResults[expr] = result;
    }

    return result;
}

/**
 * @brief Collects all variables in the expression tree
 * @param node Current node in the syntax tree
 * @return Set of variable names found in the tree
 */
std::set<std::string> Evaluator::collectVariables(Node* node) {
    std::set<std::string> variables;
    if (!node) return variables;

    if (node->value.length() == 1 && std::isupper(node->value[0])) {
        variables.insert(node->value);
    }

    for (auto child : node->children) {
        auto childVars = collectVariables(child);
        variables.insert(childVars.begin(), childVars.end());
    }

    return variables;
}

/**
 * @brief Generates a complete truth table for the logical expression
 * @return Pair containing the truth table data and column headers
 */
std::pair<std::vector<std::pair<std::unordered_map<std::string, bool>, std::unordered_map<std::string, bool>>>,
          std::vector<std::string>>
Evaluator::generateTruthTable() {
    try {
        std::set<std::string> variableSet = collectVariables(root);
        std::vector<std::string> variables(variableSet.begin(), variableSet.end());
        std::sort(variables.begin(), variables.end());  // Sort variables alphabetically
        std::string fullExpression = nodeToString(root);

        // Generate truth table
        std::vector<std::pair<std::unordered_map<std::string, bool>,
                             std::unordered_map<std::string, bool>>> table;
        std::vector<std::string> subExpressions;

        // Generate first row to get all subexpressions
        std::unordered_map<std::string, bool> firstValues;
        for (const auto& var : variables) {
            firstValues[var] = false;
        }
        std::unordered_map<std::string, bool> firstResults;
        evaluateWithTracking(root, firstValues, firstResults);

        // Order subexpressions based on complexity
        auto orderSubExpressions = [](const std::string& a, const std::string& b) {
            // Count spaces as a measure of expression complexity
            int complexityA = std::count(a.begin(), a.end(), ' ');
            int complexityB = std::count(b.begin(), b.end(), ' ');
            if (complexityA != complexityB) return complexityA < complexityB;
            return a < b;
        };

        // Collect and order subexpressions
        for (const auto& [expr, _] : firstResults) {
            if (expr != fullExpression &&
                std::find(variables.begin(), variables.end(), expr) == variables.end()) {
                subExpressions.push_back(expr);
            }
        }
        std::sort(subExpressions.begin(), subExpressions.end(), orderSubExpressions);

        // Generate all combinations
        size_t numCombinations = std::pow(2, variables.size());
        for (size_t i = 0; i < numCombinations; ++i) {
            std::unordered_map<std::string, bool> values;
            for (size_t j = 0; j < variables.size(); ++j) {
                values[variables[j]] = (i & (1 << j)) != 0;
            }

            std::unordered_map<std::string, bool> subResults;
            evaluateWithTracking(root, values, subResults);
            table.emplace_back(values, subResults);
        }

        // Prepare final column order
        std::vector<std::string> finalColumns = variables;
        finalColumns.insert(finalColumns.end(), subExpressions.begin(), subExpressions.end());
        finalColumns.push_back(fullExpression);

        return {table, finalColumns};
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to generate truth table: " + std::string(e.what()));
    }
}

/**
 * @brief Converts a node and its subtree to a string representation
 * @param node Current node in the syntax tree
 * @return String representation of the logical expression
 */
std::string Evaluator::nodeToString(Node* node) {
    if (!node) return "";

    if (node->children.empty()) {
        return node->value;
    }

    if (node->value == "NOT") {
        return "NOT " + nodeToString(node->children[0]);
    }

    if (node->children.size() == 2) {
        // Add parentheses for compound expressions
        std::string left = nodeToString(node->children[0]);
        std::string right = nodeToString(node->children[1]);

        // Add parentheses if the child is a compound expression
        if (node->children[0]->children.size() == 2) {
            left = "(" + left + ")";
        }
        if (node->children[1]->children.size() == 2 ||
            (node->children[1]->value == "NOT" && node->children[1]->children.size() == 1)) {
            right = "(" + right + ")";
        }

        return left + " " + node->value + " " + right;
    }

    return node->value;
}

/**
 * @brief Prints the syntax tree in a hierarchical format
 * @param node Current node in the syntax tree
 * @param level Current depth in the tree for indentation
 */
void print_tree(Node* node, int level) {
    if (node == nullptr) return;
    std::cout << std::string(level * 2, ' ') << "Node: ";
    std::cout << node->value << std::endl;  // Simplified for demonstration
    for (auto child : node->children) {
        print_tree(child, level + 1);
    }
}
