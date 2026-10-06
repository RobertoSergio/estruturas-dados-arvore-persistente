#ifndef NODE_HPP
#define NODE_HPP

struct Node {
    int value;
    Node* left;
    Node* right;

    explicit Node(int value)
        : value(value), left(nullptr), right(nullptr) {
    }
};

#endif