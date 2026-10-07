#ifndef NODE_HPP
#define NODE_HPP

#include <array>
#include <cstddef>

struct Node;

struct ChildState {
    int version = 0;
    Node* left = nullptr;
    Node* right = nullptr;
};

struct Node {
    static constexpr std::size_t HistoryCapacity = 2;

    int value;
    Node* baseLeft;
    Node* baseRight;

    std::array<ChildState, HistoryCapacity> history{};
    std::size_t historySize = 0;

    Node(
        int value,
        Node* left = nullptr,
        Node* right = nullptr
    )
        : value(value),
          baseLeft(left),
          baseRight(right) {
    }
};

#endif