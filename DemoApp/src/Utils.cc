#include <iostream>
#include <limits>

#include "Utils.h"

void Utils::clear() {
    std::cout << clearScreen();
}

void Utils::printTitle(std::string title) {
    std::cout << "\n\t" << setFormatting({ConsoleFormat::BOLD, ConsoleFormat::T_GREEN}) << title << clearFormatting()
              << "\n\n";
}

void Utils::invalidValue(std::string info) {
    std::cout << setFormatting({ConsoleFormat::T_RED}) << info << clearFormatting();
    waitForEnter();
}

void Utils::waitForEnter() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// std::vector<std::string> splitString(const string &s, char delim) {
//     std::vector<std::string> result;
//     std::stringstream ss(s);
//     std::string item;
//
//     while (getline(ss, item, delim)) {
//         result.push_back(item);
//     }
//
//     return result;
// }

void Utils::printWrapped(const std::string &s) {
    // 	std::vector<std::string> words = splitString(s, ' ');
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
    int v = 0;
    double d = 0;
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
    if (arguments.empty()) {
        return true;
    }

    bool allSpecified = true;

    for (;;) {
        std::cout << setCursor(4, 0) << clearLinesBelow();
        allSpecified = true;

        std::cout << "You need to specify following arguments:\n";

        for (const Argument &a : arguments) {
            allSpecified &= a.valueSet;
            printArgument(a);
        }

        size_t option = printChooseArgumentsOptions(arguments, allSpecified);

        if (option == 0) {
            return false;
        }

        if (allSpecified && (option == arguments.size() + 1)) {
            return true;
        }

        changeArgument(arguments[option - 1]);
    }
}
