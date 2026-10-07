#include "PersistentBST.hpp"

#include <iostream>
#include <vector>

Node* PersistentBST::findCurrentNode(int value) const {
    Node* current = persistence.currentRoot();

    while (current != nullptr) {
        if (current->value == value) {
            return current;
        }

        if (value < current->value) {
            current = persistence.currentLeft(current);
        } else {
            current = persistence.currentRight(current);
        }
    }

    return nullptr;
}

Node* PersistentBST::minimum(Node* node) const {
    Node* current = node;

    while (current != nullptr) {
        Node* left = persistence.currentLeft(current);

        if (left == nullptr) {
            break;
        }

        current = left;
    }

    return current;
}

void PersistentBST::removeWithTwoChildren(Node* node) {
    // Usa o menor elemento da subárvore direita como sucessor.
    Node* right = persistence.currentRight(node);
    Node* successorNode = minimum(right);

    int successorValue = successorNode->value;
    Node* successorRight = persistence.currentRight(successorNode);

    Node* newRight = nullptr;

    if (successorNode == right) {
        newRight = successorRight;
    } else {
        /*
         * O sucessor é o menor da subárvore direita e,
         * portanto, não possui filho esquerdo.
         */
        persistence.replace(successorNode, successorRight);

        // A operação pode ter criado novas cópias no caminho até node.
        node = persistence.currentNode(node);
        newRight = persistence.currentRight(node);
    }

    node = persistence.currentNode(node);

    Node* replacement = persistence.createNode(
        successorValue,
        persistence.currentLeft(node),
        newRight
    );

    /*
     * Criamos outro nó em vez de alterar o valor existente,
     * pois versões antigas ainda podem apontar para o nó original.
     */
    persistence.replace(node, replacement);
}

void PersistentBST::insert(int value) {
    persistence.beginVersion();

    Node* newNode = persistence.createNode(value);

    if (persistence.currentRoot() == nullptr) {
        persistence.setRoot(newNode);
        persistence.commitVersion();
        return;
    }

    Node* current = persistence.currentRoot();

    while (true) {
        // Valores repetidos são inseridos na subárvore direita.
        Branch branch =
            value < current->value
                ? Branch::Left
                : Branch::Right;

        Node* next =
            branch == Branch::Left
                ? persistence.currentLeft(current)
                : persistence.currentRight(current);

        if (next == nullptr) {
            persistence.setChild(current, branch, newNode);
            break;
        }

        current = next;
    }

    persistence.commitVersion();
}

void PersistentBST::remove(int value) {
    persistence.beginVersion();

    Node* target = findCurrentNode(value);

    // REM também cria versão quando o valor não existe.
    if (target == nullptr) {
        persistence.commitVersion();
        return;
    }

    Node* left = persistence.currentLeft(target);
    Node* right = persistence.currentRight(target);

    if (left == nullptr) {
        persistence.replace(target, right);
    } else if (right == nullptr) {
        persistence.replace(target, left);
    } else {
        removeWithTwoChildren(target);
    }

    persistence.commitVersion();
}

bool PersistentBST::successor(
    int value,
    int version,
    int& result
) const {
    int actualVersion = persistence.normalizeVersion(version);

    Node* current = persistence.rootAt(actualVersion);
    Node* candidate = nullptr;

    /*
     * Um valor maior vira candidato. A busca continua pela esquerda
     * para verificar se existe outro candidato menor.
     */
    while (current != nullptr) {
        if (current->value > value) {
            candidate = current;
            current = persistence.leftAt(current, actualVersion);
        } else {
            current = persistence.rightAt(current, actualVersion);
        }
    }

    if (candidate == nullptr) {
        return false;
    }

    result = candidate->value;
    return true;
}

void PersistentBST::print(int version) const {
    int actualVersion = persistence.normalizeVersion(version);

    Node* current = persistence.rootAt(actualVersion);
    std::vector<Node*> stack;

    bool first = true;

    // Percurso em ordem iterativo.
    while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
            stack.push_back(current);
            current = persistence.leftAt(current, actualVersion);
        }

        current = stack.back();
        stack.pop_back();

        if (!first) {
            std::cout << ' ';
        }

        std::cout << current->value;
        first = false;

        current = persistence.rightAt(current, actualVersion);
    }

    std::cout << '\n';
}

int PersistentBST::latestVersion() const {
    return persistence.latestVersion();
}