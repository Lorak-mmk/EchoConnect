#ifndef DEMOAPP_ARGUMENT_H
#define DEMOAPP_ARGUMENT_H

#include <variant>

enum ArgumentType { INTEGER, DOUBLE, STRING };

typedef struct Argument {
    ArgumentType type;
    std::string name;
    bool valueSet;
    std::variant<std::string, int, double> value;

    Argument(std::string n, int v) : name(n), value(v) {
        type = ArgumentType::INTEGER;
        valueSet = false;
    }

    Argument(std::string n, double v) : name(n), value(v) {
        type = ArgumentType::DOUBLE;
        valueSet = false;
    }

    Argument(std::string n, std::string v) : name(n), value(v) {
        type = ArgumentType::STRING;
        valueSet = false;
    }

} Argument;

#endif  // DEMOAPP_ARGUMENT_H