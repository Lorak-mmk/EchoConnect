#include <iostream>
#include <limits>

#include "Utils.h"

void Utils::clear() {
    std::cout << clearScreen();
}

void Utils::printTitle(const std::string &title) {
    std::cout << setFormatting({ConsoleFormat::BOLD, ConsoleFormat::T_GREEN}) << "\n " << title << "\n\n"
              << clearFormatting();
}

void Utils::printOption(size_t nr, const std::string &name) {
    std::cout << " " << nr << ". " << name << "\n";
}

void Utils::invalidValue(const std::string &info) {
    std::cout << setFormatting({ConsoleFormat::T_RED}) << info << clearFormatting();
    waitForEnter();
}

void Utils::waitForEnter() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

size_t printChooseArgumentsOptions(const std::vector<Argument> &arguments, bool allSpecified) {
    size_t biggestOption = arguments.size();

    size_t i = 1;
    for (const Argument &a : arguments) {
        Utils::printOption(i, "Change " + a.name);
        i++;
    }

    if (allSpecified) {
        Utils::printOption(arguments.size() + 1, "Run");
        biggestOption++;
    }

    Utils::printOption(0, "Go back");

    std::string prompt = setFormatting({ConsoleFormat::T_YELLOW}) + " Choose: " + clearFormatting();

    while ((i = Utils::readValue<size_t>(prompt)) > biggestOption) {
        Utils::invalidValue(" Not a valid option, press enter to continue...");
        std::cout << cursorUp(3) << clearLinesBelow();
    }

    return i;
}

void changeArgument(Argument &a) {
    std::cout << setCursor(4, 0) << clearLinesBelow();
    a.print();

    std::string prompt = setFormatting({ConsoleFormat::T_YELLOW}) + " Enter new value: " + clearFormatting();

    switch (a.type) {
        case INTEGER:
            a.value = Utils::readValue<int>(prompt);
            break;
        case DOUBLE:
            a.value = Utils::readValue<double>(prompt);
            break;
        case STRING:
            a.value = Utils::readValue<std::string>(prompt);
            break;
        default:
            break;
    }

    a.valueSet = true;
}

bool Utils::readArguments(std::vector<Argument> &arguments) {
    if (arguments.empty()) {
        return true;
    }

    bool allSpecified = true;

    for (;;) {
        std::cout << setCursor(4, 0) << clearLinesBelow();
        allSpecified = true;

        std::cout << setFormatting({ConsoleFormat::T_BLUE}) << " You may specify following arguments:\n\n"
                  << clearFormatting();

        for (const Argument &a : arguments) {
            allSpecified &= a.valueSet;
            a.print();
        }

        std::cout << "\n\n";
        size_t option = printChooseArgumentsOptions(arguments, allSpecified);

        if (option == 0) {
            return false;
        }

        if (option == arguments.size() + 1) {
            return true;
        }

        changeArgument(arguments[option - 1]);
    }
}
