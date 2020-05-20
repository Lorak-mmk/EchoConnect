#include <fstream>
#include <iostream>
#include <limits>

#include "Utils.h"

#ifdef _WIN32
#include <winsock2.h>
#elif defined __unix__
#include <arpa/inet.h>
#endif


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
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

size_t printChooseArgumentsOptions(const std::map<std::string, Argument> &arguments, bool allSpecified) {
    size_t biggestOption = arguments.size();

    size_t i = 1;
    for (const auto &[key, arg] : arguments) {
        Utils::printOption(i, "Change " + arg.title);
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
        case REAL:
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

bool Utils::readArguments(std::map<std::string, Argument> &arguments) {
    if (arguments.empty()) {
        return true;
    }

    bool allSpecified = true;

    for (;;) {
        std::cout << setCursor(4, 0) << clearLinesBelow();
        allSpecified = true;

        std::cout << setFormatting({ConsoleFormat::T_BLUE}) << " You may specify following arguments:\n\n"
                  << clearFormatting();

        for (auto const &[key, arg] : arguments) {
            allSpecified &= arg.valueSet;
            arg.print();
        }

        std::cout << "\n\n";
        size_t option = printChooseArgumentsOptions(arguments, allSpecified);

        if (option == 0) {
            std::cout << setCursor(4, 0) << clearLinesBelow();
            return false;
        }

        if (option == arguments.size() + 1) {
            std::cout << setCursor(4, 0) << clearLinesBelow();
            return true;
        }

        auto it = arguments.begin();
        for (size_t i = 1; i < option; i++, it++) {
        }
        changeArgument(it->second);
    }
}

size_t Utils::fileSize(std::ifstream &file) {
    file.ignore((std::numeric_limits<std::streamsize>::max)());
    std::streamsize length = file.gcount();
    file.clear();
    file.seekg(0, std::ios_base::beg);

    return length;
}

#ifndef __APPLE__
uint64_t Utils::HTONLL(uint64_t x) {
    return ((1 == htonl(1)) ? (x) : (((uint64_t)htonl((x)&0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)));
}

uint64_t Utils::NTOHLL(uint64_t x) {
    return ((1 == ntohl(1)) ? (x) : (((uint64_t) ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t) ((x) >> 32)));
}
#endif
