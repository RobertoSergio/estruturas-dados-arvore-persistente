#ifndef PERSISTENT_BST_HPP
#define PERSISTENT_BST_HPP

#include "PersistenceManager.hpp"

/*
 * Implementa as operações da árvore binária de busca.
 * A parte de versionamento fica delegada ao PersistenceManager.
 */
class PersistentBST {
private:
    PersistenceManager persistence;

    Node* findCurrentNode(int value) const;

    Node* findCurrentMinimum(
        Node* node
    ) const;

    void removeNodeWithTwoChildren(
        Node* node
    );

public:
    PersistentBST() = default;

    void insert(int value);
    void remove(int value);

    bool successor(
        int value,
        int version,
        int& result
    ) const;

    void print(int version) const;

    int latestVersion() const;
};

#endif