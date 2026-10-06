#ifndef COMMAND_HPP
#define COMMAND_HPP

enum class CommandType {
    Insert,
    Remove,
    Successor,
    Print
};

struct Command {
    CommandType type;
    int value;
    int version;
};

#endif