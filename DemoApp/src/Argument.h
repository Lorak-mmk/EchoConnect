#ifndef DEMOAPP_ARGUMENT_H
#define DEMOAPP_ARGUMENT_H

#include <variant>

enum ArgumentType { INTEGER, DOUBLE, STRING };

typedef struct Argument {
    ArgumentType type;
    std::string name;
    bool hasDefault;
    std::variant<std::string, int, double> v;
} Argument;

#endif  // DEMOAPP_ARGUMENT_H