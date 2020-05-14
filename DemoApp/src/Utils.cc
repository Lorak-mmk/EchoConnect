#include <iostream>

#include "Utils.h"

void Utils::clear() {
    std::cout << "\033c";
}

void printArgument(const Argument &a) {
    switch (a.type) {
        case INTEGER:
            std::cout << "integer: " << a.name;
            if (a.valueSet) {
                std::cout << " = " << std::get<int>(a.value);
            }

            std::cout << "\n";
            break;
        case DOUBLE:
            std::cout << "floating point number: " << a.name;

            if (a.valueSet) {
                std::cout << " = " << std::get<double>(a.value);
            }

            std::cout << "\n";
            break;
        case STRING:
            std::cout << "string: " << a.name;

            if (a.valueSet) {
                std::cout << " = " << std::get<std::string>(a.value);
            }

            std::cout << "\n";
            break;
        default:
            break;
    }
}

void readArgumentValue(Argument &a) {
    int v;
    double d;
    std::string s;

    // TODO: ogarnąć żeby się nie wywalało, jak ktoś podaje np. stringa zamiast inta.

    switch (a.type) {
        case INTEGER:
            std::cin >> v;
            a.value = v;
            break;
        case DOUBLE:
            std::cin >> d;
            a.value = d;
            break;
        case STRING:
            std::cin >> s;
            a.value = s;
            break;
        default:
            break;
    }

    a.valueSet = true;
}

size_t printChooseArgumentsOptions(const std::vector<Argument> &arguments, bool allSpecified) {
    std::cout << "Choose what you want to do:\n";
    std::cout << "0. Return to the previous view\n";

    size_t i = 1;
    for (const Argument &a : arguments) {
        std::cout << i << ". Change " << a.name << "\n";
        i++;
    }

    if (allSpecified) {
        std::cout << arguments.size() + 1 << ". Run\n";
    }

    std::cin >> i;

    return i;
}

void changeArgument(Argument &a) {
    Utils::clear();

    std::cout << "Current value:\n";
    printArgument(a);

    std::cout << "Enter new value: \n";
    readArgumentValue(a);
}

bool Utils::readArguments(std::vector<Argument> &arguments) {
    bool allSpecified = true;

    for (;;) {
        Utils::clear();
        allSpecified = true;

        std::cout << "You need to specify following arguments:\n";

        for (const Argument &a : arguments) {
            allSpecified &= a.valueSet;
            printArgument(a);
        }

        size_t option = printChooseArgumentsOptions(arguments, allSpecified);

        if (option == 0) {
            return false;
        } else if (allSpecified && (option == arguments.size() + 1)) {
            return true;
        } else {
            changeArgument(arguments[option - 1]);
        }
    }
}