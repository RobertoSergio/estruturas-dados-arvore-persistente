#include "InputParser.hpp"

#include <string>

InputParser::InputParser(const std::string& filePath)
    : input(filePath) {
}

bool InputParser::isOpen() const {
    return input.is_open();
}

bool InputParser::nextCommand(Command& command) {
    std::string commandName;

    if (!(input >> commandName)) {
        return false;
    }

    if (commandName == "INC") {
        int value;

        if (!(input >> value)) {
            return false;
        }

        command.type = CommandType::Insert;
        command.value = value;
        command.version = 0;

        return true;
    }

    if (commandName == "REM") {
        int value;

        if (!(input >> value)) {
            return false;
        }

        command.type = CommandType::Remove;
        command.value = value;
        command.version = 0;

        return true;
    }

    if (commandName == "SUC") {
        int value;
        int version;

        if (!(input >> value >> version)) {
            return false;
        }

        command.type = CommandType::Successor;
        command.value = value;
        command.version = version;

        return true;
    }

    if (commandName == "IMP") {
        int version;

        if (!(input >> version)) {
            return false;
        }

        command.type = CommandType::Print;
        command.value = 0;
        command.version = version;

        return true;
    }

    return false;
}