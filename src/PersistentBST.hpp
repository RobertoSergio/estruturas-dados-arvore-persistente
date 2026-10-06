#ifndef PERSISTENT_BST_HPP
#define PERSISTENT_BST_HPP

#include "Node.hpp"

class PersistentBST {
private:
    Node* root;

    Node* insertNode(Node* node, int value);
    void printInOrder(Node* node);
    void destroyTree(Node* node);

public:
    PersistentBST();
    ~PersistentBST();

    void insert(int value);
    void print();
};

#endif