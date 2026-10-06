#include "PersistentBST.hpp"

#include <iostream>

PersistentBST::PersistentBST()
    : root(nullptr) {
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

Node* PersistentBST::findMin(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    while (node->left != nullptr) {
        node = node->left;
    }

    return node;
}

Node* PersistentBST::removeNode(Node* node, int value) {
    if (node == nullptr) {
        return nullptr;
    }

    if (value < node->value) {
        node->left = removeNode(node->left, value);
    } else if (value > node->value) {
        node->right = removeNode(node->right, value);
    } else {
        if (node->left == nullptr) {
            Node* rightChild = node->right;
            delete node;
            return rightChild;
        }

        if (node->right == nullptr) {
            Node* leftChild = node->left;
            delete node;
            return leftChild;
        }

        Node* successor = findMin(node->right);

        node->value = successor->value;
        node->right = removeNode(node->right, successor->value);
    }

    return node;
}

void PersistentBST::remove(int value) {
    root = removeNode(root, value);
}

Node* PersistentBST::findSuccessor(Node* node, int value) {
    Node* candidate = nullptr;

    while (node != nullptr) {
        if (node->value > value) {
            candidate = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return candidate;
}

bool PersistentBST::successor(int value, int& result) {
    Node* node = findSuccessor(root, value);

    if (node == nullptr) {
        return false;
    }

    result = node->value;
    return true;
}

void PersistentBST::printInOrder(Node* node, bool& first) {
    if (node == nullptr) {
        return;
    }

    printInOrder(node->left, first);

    if (!first) {
        std::cout << " ";
    }

    std::cout << node->value;
    first = false;

    printInOrder(node->right, first);
}

void PersistentBST::print() {
    bool first = true;

    printInOrder(root, first);

    std::cout << '\n';
}

void PersistentBST::destroyTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);

    delete node;
}