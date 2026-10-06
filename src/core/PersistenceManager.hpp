#ifndef PERSISTENCE_MANAGER_HPP
#define PERSISTENCE_MANAGER_HPP

#include <memory>
#include <vector>

#include "Node.hpp"

/*
 * Responsável pelo mecanismo de persistência:
 * versões, histórico de alterações e cópia de nós.
 */
class PersistenceManager {
private:
    // Mantém a propriedade dos nós criados durante a execução.
    std::vector<std::unique_ptr<Node>> nodes;

    // roots[v] guarda a raiz correspondente à versão v.
    std::vector<Node*> roots;

    Node* currentRoot;
    int currentVersion;

    bool hasModificationSpace(Node* node) const;

    void addModification(
        Node* node,
        ChildSide side,
        Node* child
    );

    Node* applyChildChange(
        Node* node,
        ChildSide side,
        Node* child
    );

public:
    PersistenceManager();

    Node* createNode(
        int value,
        Node* left = nullptr,
        Node* right = nullptr
    );

    void beginVersion();
    void commitVersion();

    Node* getCurrentRoot() const;

    Node* childAt(
        Node* node,
        ChildSide side,
        int version
    ) const;

    Node* currentChild(
        Node* node,
        ChildSide side
    ) const;

    Node* latestCopy(Node* node) const;

    void setChild(
        Node* parent,
        ChildSide side,
        Node* child
    );

    void replaceNode(
        Node* oldNode,
        Node* newNode
    );

    void setRoot(Node* newRoot);

    void adoptCurrentChildren(Node* node);

    ChildSide sideOfChild(
        Node* parent,
        Node* child
    ) const;

    int resolveVersion(int requestedVersion) const;

    Node* rootAt(int version) const;

    int latestVersion() const;
};

#endif