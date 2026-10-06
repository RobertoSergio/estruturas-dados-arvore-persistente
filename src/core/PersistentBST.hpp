#ifndef PERSISTENT_BST_HPP
#define PERSISTENT_BST_HPP

#include "Node.hpp"

class PersistentBST {
private:
    Node* root;

    Node* insertNode(Node* node, int value);
    Node* removeNode(Node* node, int value);

    Node* findMin(Node* node);
    Node* findSuccessor(Node* node, int value);

    void printInOrder(Node* node, bool& first);
    void destroyTree(Node* node);

public:
    PersistentBST();
    ~PersistentBST();

    void insert(int value);
    void remove(int value);

    bool successor(int value, int& result);

    void print();
};

#endif