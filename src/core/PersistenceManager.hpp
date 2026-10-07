#ifndef PERSISTENCE_MANAGER_HPP
#define PERSISTENCE_MANAGER_HPP

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Node.hpp"

enum class Branch {
    Left,
    Right
};

class PersistenceManager {
private:
    struct ParentLink {
        Node* parent = nullptr;
        Branch branch = Branch::Left;
    };

    std::vector<std::unique_ptr<Node>> storage;
    std::vector<Node*> versionRoots;

    /*
     * Essas estruturas representam apenas informações da árvore atual.
     * Elas não fazem parte do estado persistente armazenado em Node.
     */
    std::unordered_map<Node*, ParentLink> parentLinks;
    std::unordered_map<Node*, Node*> replacements;

    Node* workingRoot = nullptr;
    int workingVersion = 0;

    std::pair<Node*, Node*> childrenAt(
        Node* node,
        int version
    ) const;

    void recordState(
        Node* node,
        Node* left,
        Node* right
    );

    void registerChildren(Node* node);

    Node* updateChildren(
        Node* node,
        Node* left,
        Node* right
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

    Node* currentRoot() const;

    Node* currentNode(
        Node* node
    ) const;

    Node* leftAt(
        Node* node,
        int version
    ) const;

    Node* rightAt(
        Node* node,
        int version
    ) const;

    Node* currentLeft(
        Node* node
    ) const;

    Node* currentRight(
        Node* node
    ) const;

    void setChild(
        Node* parent,
        Branch branch,
        Node* child
    );

    void replace(
        Node* oldNode,
        Node* newNode
    );

    void setRoot(
        Node* root
    );

    int normalizeVersion(
        int requestedVersion
    ) const;

    Node* rootAt(
        int version
    ) const;

    int latestVersion() const;
};

#endif