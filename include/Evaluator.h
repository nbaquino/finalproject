#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <cmath>
#include <utility>
#include "Node.h"

class Evaluator {
public:
    // Constructor that takes the root of the parse tree
    Evaluator(Node* root);

    // Function to evaluate a logical expression with tracking intermediate results
    bool evaluateWithTracking(Node* node, const std::unordered_map<std::string, bool>& values,
                              std::unordered_map<std::string, bool>& subResults);

    // Function to generate a truth table for the logical expression
    std::pair<std::vector<std::pair<std::unordered_map<std::string, bool>, std::unordered_map<std::string, bool>>>,
              std::vector<std::string>> generateTruthTable();

    // Add this new method declaration to the Evaluator class
    std::set<std::string> collectVariables(Node* node);

private:
    // Helper function to convert a node to its string representation
    std::string nodeToString(Node* node);

    // Root node of the parse tree
    Node* root;
};

#endif // EVALUATOR_H
