#include "PersistenceManager.hpp"

PersistenceManager::PersistenceManager()
    : workingRoot(nullptr), workingVersion(0) {
    // A versão 0 representa a árvore vazia.
    versionRoots.push_back(nullptr);
}

Node* PersistenceManager::createNode(int value, Node* left, Node* right) {
    storage.push_back(std::make_unique<Node>(value, left, right));

    Node* node = storage.back().get();
    registerChildren(node);

    return node;
}

void PersistenceManager::beginVersion() {
    workingVersion = static_cast<int>(versionRoots.size());
}

void PersistenceManager::commitVersion() {
    versionRoots.push_back(workingRoot);
}

Node* PersistenceManager::currentRoot() const {
    return workingRoot;
}

Node* PersistenceManager::currentNode(Node* node) const {
    Node* current = node;

    // Procura a representação mais recente do nó na árvore atual.
    while (current != nullptr) {
        auto it = replacements.find(current);

        if (it == replacements.end()) {
            break;
        }

        current = it->second;
    }

    return current;
}

std::pair<Node*, Node*> PersistenceManager::childrenAt(
    Node* node,
    int version
) const {
    if (node == nullptr) {
        return {nullptr, nullptr};
    }

    Node* left = node->baseLeft;
    Node* right = node->baseRight;

    // Cada entrada registra o estado dos filhos a partir daquela versão.
    for (std::size_t i = 0; i < node->historySize; ++i) {
        const ChildState& state = node->history[i];

        if (state.version <= version) {
            left = state.left;
            right = state.right;
        }
    }

    return {left, right};
}

Node* PersistenceManager::leftAt(Node* node, int version) const {
    return childrenAt(node, version).first;
}

Node* PersistenceManager::rightAt(Node* node, int version) const {
    return childrenAt(node, version).second;
}

Node* PersistenceManager::currentLeft(Node* node) const {
    return leftAt(node, workingVersion);
}

Node* PersistenceManager::currentRight(Node* node) const {
    return rightAt(node, workingVersion);
}

void PersistenceManager::recordState(
    Node* node,
    Node* left,
    Node* right
) {
    node->history[node->historySize] = {
        workingVersion,
        left,
        right
    };

    ++node->historySize;
}

void PersistenceManager::registerChildren(Node* node) {
    if (node == nullptr) {
        return;
    }

    Node* left = currentLeft(node);
    Node* right = currentRight(node);

    // parentLinks representa somente a árvore mais recente.
    if (left != nullptr) {
        parentLinks[left] = {node, Branch::Left};
    }

    if (right != nullptr) {
        parentLinks[right] = {node, Branch::Right};
    }
}

Node* PersistenceManager::updateChildren(
    Node* node,
    Node* left,
    Node* right
) {
    Node* current = currentNode(node);

    // Ainda há espaço no histórico do nó.
    if (current->historySize < Node::HistoryCapacity) {
        recordState(current, left, right);
        registerChildren(current);

        return current;
    }

    auto parentIt = parentLinks.find(current);
    bool hasParent = parentIt != parentLinks.end();

    ParentLink previousLink;

    if (hasParent) {
        previousLink = parentIt->second;
    }

    /*
     * O histórico está cheio. O estado atual é materializado
     * em um novo nó e passa a representar o antigo na árvore atual.
     */
    Node* copy = createNode(current->value, left, right);
    replacements[current] = copy;

    if (!hasParent) {
        setRoot(copy);
        return copy;
    }

    Node* parent = currentNode(previousLink.parent);

    /*
     * A atualização do pai pode, por sua vez, gerar outra cópia
     * e propagar a alteração em direção à raiz.
     */
    setChild(parent, previousLink.branch, copy);

    return copy;
}

void PersistenceManager::setChild(
    Node* parent,
    Branch branch,
    Node* child
) {
    Node* currentParent = currentNode(parent);

    Node* left = currentLeft(currentParent);
    Node* right = currentRight(currentParent);

    if (branch == Branch::Left) {
        left = child;
    } else {
        right = child;
    }

    Node* updatedParent = updateChildren(currentParent, left, right);

    if (child != nullptr) {
        parentLinks[child] = {updatedParent, branch};
    }
}

void PersistenceManager::replace(Node* oldNode, Node* newNode) {
    Node* currentOld = currentNode(oldNode);

    auto parentIt = parentLinks.find(currentOld);

    if (parentIt == parentLinks.end()) {
        setRoot(newNode);
        return;
    }

    ParentLink link = parentIt->second;
    Node* parent = currentNode(link.parent);

    setChild(parent, link.branch, newNode);
}

void PersistenceManager::setRoot(Node* root) {
    workingRoot = root;

    if (root != nullptr) {
        parentLinks.erase(root);
    }
}

int PersistenceManager::normalizeVersion(int requestedVersion) const {
    if (
        requestedVersion < 0 ||
        requestedVersion >= static_cast<int>(versionRoots.size())
    ) {
        return latestVersion();
    }

    return requestedVersion;
}

Node* PersistenceManager::rootAt(int version) const {
    return versionRoots[version];
}

int PersistenceManager::latestVersion() const {
    return static_cast<int>(versionRoots.size()) - 1;
}