// Node.h
#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class Node {
public:
    std::string value;  // The value or operator in the node
    std::vector<Node*> children;  // Children of this node

    // Constructor for a node with a specific value
    Node(const std::string& val) : value(val) {}

    // Destructor to clean up dynamically allocated children
    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }

    // Add a child node to this node
    void addChild(Node* child) {
        children.push_back(child);
    }
};

#endif // NODE_H
