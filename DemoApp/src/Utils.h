#ifndef DEMOAPP_UTILS_H
#define DEMOAPP_UTILS_H

#include <iostream>
#include <limits>
#include <map>

#include "Argument.h"
#include "Console.h"

#ifdef _WIN32
using ssize_t = long long;
#endif

class Utils {
public:
    static void clear();
    static void printTitle(const std::string &title);
    static void printOption(size_t nr, const std::string &name);

    template <typename T>
    static T readValue(const std::string &prompt) {
        T v;
        std::cout << "\n" << prompt;
        std::cin >> v;
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            invalidValue(" Please specify a valid value, press enter to continue...");
            std::cout << cursorUp(2) << clearLinesBelow() << prompt;
            std::cin >> v;
        }
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        return v;
    }
    static void invalidValue(const std::string &info);
    static void waitForEnter();
    static bool readArguments(std::map<std::string, Argument> &arguments);
    static size_t fileSize(std::ifstream &file);
};

#ifndef __APPLE__
uint64_t HTONLL(uint64_t x);
uint64_t NTOHLL(uint64_t x);
#endif

#endif  // DEMOAPP_UTILS_H