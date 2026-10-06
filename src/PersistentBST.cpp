#include "PersistentBST.hpp"

#include <iostream>

using namespace std;

PersistentBST::PersistentBST() {
    root = nullptr;
}

PersistentBST::~PersistentBST() {
    destroyTree(root);
}

Node* PersistentBST::insertNode(Node* node, int value) {
    if (node == nullptr) {
        return new Node(value);
    }

    if (value < node->value) {
        node->left = insertNode(node->left, value);
    } else {
        node->right = insertNode(node->right, value);
    }

    return node;
}

void PersistentBST::insert(int value) {
    root = insertNode(root, value);
}

void PersistentBST::printInOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    printInOrder(node->left);

    cout << node->value << " ";

    printInOrder(node->right);
}

void PersistentBST::print() {
    printInOrder(root);
    cout << endl;
}

void PersistentBST::destroyTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);

    delete node;
}