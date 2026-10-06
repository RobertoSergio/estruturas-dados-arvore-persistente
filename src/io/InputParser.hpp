#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include <fstream>
#include <string>

#include "Command.hpp"

class InputParser {
private:
    std::ifstream input;

public:
    explicit InputParser(const std::string& filePath);

    bool isOpen() const;
    bool nextCommand(Command& command);
};

#endif