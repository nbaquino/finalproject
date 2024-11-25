#include "Evaluator.h"
#include <iostream>
#include <algorithm>
#include <cmath>

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

std::pair<std::vector<std::pair<std::unordered_map<std::string, bool>, std::unordered_map<std::string, bool>>>,
          std::vector<std::string>>
Evaluator::generateTruthTable(const std::vector<std::string>& variables) {
    std::vector<std::pair<std::unordered_map<std::string, bool>, std::unordered_map<std::string, bool>>> table;
    std::set<std::string> allSubResults;

    size_t numCombinations = std::pow(2, variables.size());
    for (size_t i = 0; i < numCombinations; ++i) {
        std::unordered_map<std::string, bool> values;
        for (size_t j = 0; j < variables.size(); ++j) {
            values[variables[j]] = (i & (1 << j)) != 0;
        }

        std::unordered_map<std::string, bool> subResults;
        bool result = evaluateWithTracking(root, values, subResults);
        subResults["Result"] = result;

        table.emplace_back(values, subResults);
        for (const auto& [key, _] : subResults) {
            allSubResults.insert(key);
        }
    }

    std::vector<std::string> intermediateColumns;
    for (const auto& col : allSubResults) {
        if (col != "Result") {
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
    finalColumns.push_back("Result");

    return {table, finalColumns};
}

std::string Evaluator::nodeToString(Node* node) {
    if (!node->children.empty()) {
        return nodeToString(node->children[0]);
    }
    return node->value;
}
