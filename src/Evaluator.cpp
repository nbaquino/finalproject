#include <iostream>
#include <algorithm>
#include <cmath>
#include "../include/Evaluator.h"

Evaluator::Evaluator(Node* root) {
    this->root = root;
}

bool Evaluator::evaluateWithTracking(Node* node, const std::unordered_map<std::string, bool>& values,
                                     std::unordered_map<std::string, bool>& subResults) {
    if (node->value == "TRUE") {
        return true;
    } else if (node->value == "FALSE") {
        return false;
    } else if (values.count(node->value)) {
        return values.at(node->value);
    } else if (node->value == "NOT") {
        bool leftResult = evaluateWithTracking(node->children[0], values, subResults);
        subResults["NOT " + nodeToString(node->children[0])] = !leftResult;
        return !leftResult;
    } else {
        bool leftResult = evaluateWithTracking(node->children[0], values, subResults);
        bool rightResult = evaluateWithTracking(node->children[1], values, subResults);
        bool result = false;

        if (node->value == "AND") {
            result = leftResult && rightResult;
        } else if (node->value == "OR") {
            result = leftResult || rightResult;
        } else if (node->value == "IMPLIES") {
            result = !leftResult || rightResult;
        } else if (node->value == "EQUIVALENT") {
            result = leftResult == rightResult;
        }

        std::string expression =
            "(" + nodeToString(node->children[0]) + " " + node->value + " " + nodeToString(node->children[1]) + ")";
        subResults[expression] = result;
        return result;
    }
}

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

std::pair<std::vector<std::pair<std::unordered_map<std::string, bool>, std::unordered_map<std::string, bool>>>,
          std::vector<std::string>>
Evaluator::generateTruthTable() {
    std::set<std::string> variableSet = collectVariables(root);
    std::vector<std::string> variables(variableSet.begin(), variableSet.end());

    std::vector<std::pair<std::unordered_map<std::string, bool>, std::unordered_map<std::string, bool>>> table;
    std::set<std::string> allSubResults;

    std::string fullExpression = nodeToString(root);

    size_t numCombinations = std::pow(2, variables.size());
    for (size_t i = 0; i < numCombinations; ++i) {
        std::unordered_map<std::string, bool> values;
        for (size_t j = 0; j < variables.size(); ++j) {
            values[variables[j]] = (i & (1 << j)) != 0;
        }

        std::unordered_map<std::string, bool> subResults;
        bool result = evaluateWithTracking(root, values, subResults);
        subResults[fullExpression] = result;

        table.emplace_back(values, subResults);
        for (const auto& [key, _] : subResults) {
            allSubResults.insert(key);
        }
    }

    std::vector<std::string> intermediateColumns;
    for (const auto& col : allSubResults) {
        if (col != fullExpression) {
            intermediateColumns.push_back(col);
        }
    }

    // Sort intermediate columns by complexity (nesting depth, then length)
    std::sort(intermediateColumns.begin(), intermediateColumns.end(), [](const std::string& a, const std::string& b) {
        return std::make_pair(std::count(a.begin(), a.end(), '('), a.size()) <
               std::make_pair(std::count(b.begin(), b.end(), '('), b.size());
    });

    std::vector<std::string> finalColumns = variables;
    finalColumns.insert(finalColumns.end(), intermediateColumns.begin(), intermediateColumns.end());
    finalColumns.push_back(fullExpression);

    return {table, finalColumns};
}

std::string Evaluator::nodeToString(Node* node) {
    if (!node) return "";

    if (node->children.empty()) {
        return node->value;
    }

    if (node->value == "NOT") {
        return "NOT " + nodeToString(node->children[0]);
    }

    if (node->children.size() == 2) {
        return "(" + nodeToString(node->children[0]) + " " + node->value + " " + nodeToString(node->children[1]) + ")";
    }

    return node->value;
}

void print_tree(Node* node, int level) {
    if (node == nullptr) return;
    std::cout << std::string(level * 2, ' ') << "Node: ";
    std::cout << node->value << std::endl;  // Simplified for demonstration
    for (auto child : node->children) {
        print_tree(child, level + 1);
    }
}
