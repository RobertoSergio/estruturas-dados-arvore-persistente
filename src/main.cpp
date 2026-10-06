#include <iostream>

#include "PersistentBST.hpp"

using namespace std;

int main() {
    PersistentBST tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(7);
    tree.insert(10);

    tree.print();

    return 0;
}