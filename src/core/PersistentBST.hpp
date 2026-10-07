#ifndef PERSISTENT_BST_HPP
#define PERSISTENT_BST_HPP

#include "PersistenceManager.hpp"

class PersistentBST {
private:
    PersistenceManager persistence;

    Node* findCurrentNode(
        int value
    ) const;

    Node* minimum(
        Node* node
    ) const;

    void removeWithTwoChildren(
        Node* node
    );

public:
    PersistentBST() = default;

    void insert(
        int value
    );

    void remove(
        int value
    );

    bool successor(
        int value,
        int version,
        int& result
    ) const;

    void print(
        int version
    ) const;

    int latestVersion() const;
};

#endif