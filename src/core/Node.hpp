#ifndef NODE_HPP
#define NODE_HPP

#include <array>
#include <cstddef>

enum class ChildSide {
    Left,
    Right
};

struct Node;

struct Modification {
    int version = 0;
    ChildSide side = ChildSide::Left;
    Node* child = nullptr;
};

struct Node {
    // Cada nó guarda um pequeno histórico de alterações.
    // Quando esse limite é atingido, uma nova cópia do nó é criada.
    static constexpr std::size_t ModificationCapacity = 2;

    int value;

    Node* initialLeft;
    Node* initialRight;

    std::array<Modification, ModificationCapacity> modifications{};
    std::size_t modificationCount = 0;

    // O ponteiro para o pai representa apenas a versão mais recente.
    Node* parent = nullptr;

    // Aponta para uma cópia mais nova do nó, caso ela exista.
    Node* newerCopy = nullptr;

    Node(
        int value,
        Node* left = nullptr,
        Node* right = nullptr
    )
        : value(value),
          initialLeft(left),
          initialRight(right) {
    }
};

#endif