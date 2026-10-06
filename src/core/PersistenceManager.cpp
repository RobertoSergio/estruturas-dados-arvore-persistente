#include "PersistenceManager.hpp"

PersistenceManager::PersistenceManager()
    : currentRoot(nullptr),
      currentVersion(0) {
    // A versão 0 representa a árvore vazia.
    roots.push_back(nullptr);
}

Node* PersistenceManager::createNode(
    int value,
    Node* left,
    Node* right
) {
    nodes.push_back(
        std::make_unique<Node>(
            value,
            left,
            right
        )
    );

    return nodes.back().get();
}

void PersistenceManager::beginVersion() {
    currentVersion =
        static_cast<int>(roots.size());
}

void PersistenceManager::commitVersion() {
    roots.push_back(currentRoot);
}

Node* PersistenceManager::getCurrentRoot() const {
    return currentRoot;
}

Node* PersistenceManager::childAt(
    Node* node,
    ChildSide side,
    int version
) const {
    if (node == nullptr) {
        return nullptr;
    }

    Node* result =
        (side == ChildSide::Left)
            ? node->initialLeft
            : node->initialRight;

    // Aplica somente as modificações que já existiam na versão consultada.
    for (
        std::size_t i = 0;
        i < node->modificationCount;
        ++i
    ) {
        const Modification& modification =
            node->modifications[i];

        if (
            modification.version <= version &&
            modification.side == side
        ) {
            result = modification.child;
        }
    }

    return result;
}

Node* PersistenceManager::currentChild(
    Node* node,
    ChildSide side
) const {
    return childAt(
        node,
        side,
        currentVersion
    );
}

Node* PersistenceManager::latestCopy(Node* node) const {
    Node* current = node;

    // Um nó pode ter sido copiado mais de uma vez ao longo das versões.
    while (
        current != nullptr &&
        current->newerCopy != nullptr
    ) {
        current = current->newerCopy;
    }

    return current;
}

bool PersistenceManager::hasModificationSpace(
    Node* node
) const {
    return
        node->modificationCount <
        Node::ModificationCapacity;
}

void PersistenceManager::addModification(
    Node* node,
    ChildSide side,
    Node* child
) {
    Modification modification;

    modification.version = currentVersion;
    modification.side = side;
    modification.child = child;

    node->modifications[
        node->modificationCount
    ] = modification;

    ++node->modificationCount;
}

Node* PersistenceManager::applyChildChange(
    Node* node,
    ChildSide side,
    Node* child
) {
    Node* liveNode = latestCopy(node);

    // Enquanto houver espaço, a alteração fica registrada no próprio nó.
    if (hasModificationSpace(liveNode)) {
        addModification(
            liveNode,
            side,
            child
        );

        return liveNode;
    }

    // Histórico cheio: cria uma cópia com o estado atual do nó.
    Node* oldParent = liveNode->parent;

    Node* left =
        (side == ChildSide::Left)
            ? child
            : currentChild(
                liveNode,
                ChildSide::Left
            );

    Node* right =
        (side == ChildSide::Right)
            ? child
            : currentChild(
                liveNode,
                ChildSide::Right
            );

    Node* copy = createNode(
        liveNode->value,
        left,
        right
    );

    liveNode->newerCopy = copy;

    adoptCurrentChildren(copy);

    /*
     * A cópia precisa ocupar a posição do nó antigo na árvore atual.
     * Se necessário, a alteração pode continuar sendo propagada para o pai.
     */
    if (oldParent == nullptr) {
        setRoot(copy);
    } else {
        Node* liveParent =
            latestCopy(oldParent);

        ChildSide occupiedSide =
            sideOfChild(
                liveParent,
                liveNode
            );

        setChild(
            liveParent,
            occupiedSide,
            copy
        );
    }

    return copy;
}

void PersistenceManager::setChild(
    Node* parent,
    ChildSide side,
    Node* child
) {
    Node* updatedParent =
        applyChildChange(
            parent,
            side,
            child
        );

    if (child != nullptr) {
        child->parent = updatedParent;
    }
}

void PersistenceManager::replaceNode(
    Node* oldNode,
    Node* newNode
) {
    Node* liveOldNode =
        latestCopy(oldNode);

    Node* parent =
        liveOldNode->parent;

    if (parent == nullptr) {
        setRoot(newNode);
        return;
    }

    Node* liveParent =
        latestCopy(parent);

    ChildSide side =
        sideOfChild(
            liveParent,
            liveOldNode
        );

    setChild(
        liveParent,
        side,
        newNode
    );
}

void PersistenceManager::setRoot(Node* newRoot) {
    currentRoot = newRoot;

    if (newRoot != nullptr) {
        newRoot->parent = nullptr;
    }
}

void PersistenceManager::adoptCurrentChildren(
    Node* node
) {
    Node* left =
        currentChild(
            node,
            ChildSide::Left
        );

    Node* right =
        currentChild(
            node,
            ChildSide::Right
        );

    // Os ponteiros parent são usados somente para a árvore atual.
    if (left != nullptr) {
        left->parent = node;
    }

    if (right != nullptr) {
        right->parent = node;
    }
}

ChildSide PersistenceManager::sideOfChild(
    Node* parent,
    Node* child
) const {
    if (
        currentChild(
            parent,
            ChildSide::Left
        ) == child
    ) {
        return ChildSide::Left;
    }

    return ChildSide::Right;
}

int PersistenceManager::resolveVersion(
    int requestedVersion
) const {
    // Versões inexistentes devem usar a versão mais recente.
    if (
        requestedVersion < 0 ||
        requestedVersion >=
            static_cast<int>(roots.size())
    ) {
        return latestVersion();
    }

    return requestedVersion;
}

Node* PersistenceManager::rootAt(
    int version
) const {
    return roots[version];
}

int PersistenceManager::latestVersion() const {
    return
        static_cast<int>(roots.size()) - 1;
}