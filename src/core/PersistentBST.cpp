#include "PersistentBST.hpp"

#include <iostream>
#include <vector>

Node* PersistentBST::findCurrentNode(
    int value
) const {
    Node* current =
        persistence.getCurrentRoot();

    while (current != nullptr) {
        if (current->value == value) {
            return current;
        }

        if (value < current->value) {
            current =
                persistence.currentChild(
                    current,
                    ChildSide::Left
                );
        } else {
            current =
                persistence.currentChild(
                    current,
                    ChildSide::Right
                );
        }
    }

    return nullptr;
}

Node* PersistentBST::findCurrentMinimum(
    Node* node
) const {
    Node* current = node;

    while (current != nullptr) {
        Node* left =
            persistence.currentChild(
                current,
                ChildSide::Left
            );

        if (left == nullptr) {
            break;
        }

        current = left;
    }

    return current;
}

void PersistentBST::removeNodeWithTwoChildren(
    Node* node
) {
    // Para dois filhos, usamos o menor nó da subárvore direita.
    Node* rightChild =
        persistence.currentChild(
            node,
            ChildSide::Right
        );

    Node* successorNode =
        findCurrentMinimum(
            rightChild
        );

    Node* successorRight =
        persistence.currentChild(
            successorNode,
            ChildSide::Right
        );

    int successorValue =
        successorNode->value;

    Node* newRight;

    if (successorNode == rightChild) {
        newRight = successorRight;
    } else {
        /*
         * O sucessor não possui filho esquerdo, então pode ser substituído
         * diretamente pelo seu filho direito.
         */
        persistence.replaceNode(
            successorNode,
            successorRight
        );

        // A operação acima pode ter criado uma cópia deste nó.
        node =
            persistence.latestCopy(node);

        newRight =
            persistence.currentChild(
                node,
                ChildSide::Right
            );
    }

    node =
        persistence.latestCopy(node);

    Node* newLeft =
        persistence.currentChild(
            node,
            ChildSide::Left
        );

    Node* replacement =
        persistence.createNode(
            successorValue,
            newLeft,
            newRight
        );

    persistence.adoptCurrentChildren(
        replacement
    );

    persistence.replaceNode(
        node,
        replacement
    );
}

void PersistentBST::insert(int value) {
    persistence.beginVersion();

    Node* newNode =
        persistence.createNode(value);

    if (
        persistence.getCurrentRoot() ==
        nullptr
    ) {
        persistence.setRoot(newNode);
        persistence.commitVersion();

        return;
    }

    Node* current =
        persistence.getCurrentRoot();

    while (true) {
        // Valores repetidos seguem para a subárvore direita.
        ChildSide side =
            (value < current->value)
                ? ChildSide::Left
                : ChildSide::Right;

        Node* next =
            persistence.currentChild(
                current,
                side
            );

        if (next == nullptr) {
            persistence.setChild(
                current,
                side,
                newNode
            );

            break;
        }

        current = next;
    }

    persistence.commitVersion();
}

void PersistentBST::remove(int value) {
    persistence.beginVersion();

    Node* target =
        findCurrentNode(value);

    // REM cria uma nova versão mesmo quando o valor não existe.
    if (target == nullptr) {
        persistence.commitVersion();
        return;
    }

    Node* left =
        persistence.currentChild(
            target,
            ChildSide::Left
        );

    Node* right =
        persistence.currentChild(
            target,
            ChildSide::Right
        );

    if (left == nullptr) {
        persistence.replaceNode(
            target,
            right
        );
    } else if (right == nullptr) {
        persistence.replaceNode(
            target,
            left
        );
    } else {
        removeNodeWithTwoChildren(
            target
        );
    }

    persistence.commitVersion();
}

bool PersistentBST::successor(
    int value,
    int version,
    int& result
) const {
    int actualVersion =
        persistence.resolveVersion(
            version
        );

    Node* current =
        persistence.rootAt(
            actualVersion
        );

    Node* candidate = nullptr;

    /*
     * Ao encontrar um valor maior, ele vira candidato e a busca segue
     * pela esquerda tentando encontrar um sucessor ainda menor.
     */
    while (current != nullptr) {
        if (current->value > value) {
            candidate = current;

            current =
                persistence.childAt(
                    current,
                    ChildSide::Left,
                    actualVersion
                );
        } else {
            current =
                persistence.childAt(
                    current,
                    ChildSide::Right,
                    actualVersion
                );
        }
    }

    if (candidate == nullptr) {
        return false;
    }

    result = candidate->value;

    return true;
}

void PersistentBST::print(
    int version
) const {
    int actualVersion =
        persistence.resolveVersion(
            version
        );

    Node* current =
        persistence.rootAt(
            actualVersion
        );

    std::vector<Node*> stack;

    bool first = true;

    // Percurso em ordem para imprimir os valores em ordem crescente.
    while (
        current != nullptr ||
        !stack.empty()
    ) {
        while (current != nullptr) {
            stack.push_back(current);

            current =
                persistence.childAt(
                    current,
                    ChildSide::Left,
                    actualVersion
                );
        }

        current = stack.back();
        stack.pop_back();

        if (!first) {
            std::cout << ' ';
        }

        std::cout << current->value;

        first = false;

        current =
            persistence.childAt(
                current,
                ChildSide::Right,
                actualVersion
            );
    }

    std::cout << '\n';
}

int PersistentBST::latestVersion() const {
    return persistence.latestVersion();
}