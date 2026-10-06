#include <iostream>

#include "core/PersistentBST.hpp"
#include "io/Command.hpp"
#include "io/InputParser.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Arquivo de entrada nao informado.\n";
        return 1;
    }

    InputParser parser(argv[1]);

    if (!parser.isOpen()) {
        std::cerr << "Nao foi possivel abrir o arquivo.\n";
        return 1;
    }

    PersistentBST tree;
    Command command;

    while (parser.nextCommand(command)) {
        switch (command.type) {
            case CommandType::Insert:
                tree.insert(command.value);
                break;

            case CommandType::Remove:
                tree.remove(command.value);
                break;

            case CommandType::Successor: {
                std::cout
                    << "SUC "
                    << command.value
                    << " "
                    << command.version
                    << '\n';

                int result;

                if (tree.successor(command.value, result)) {
                    std::cout << result << '\n';
                } else {
                    std::cout << "inf\n";
                }

                break;
            }

            case CommandType::Print:
                std::cout
                    << "IMP "
                    << command.version
                    << '\n';

                tree.print();
                break;
        }
    }

    return 0;
}