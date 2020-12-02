#ifndef DEMOAPP_UTILS_H
#define DEMOAPP_UTILS_H

#include <iostream>
#include <limits>
#include <map>
#include <sstream>

#include "Argument.h"
#include "Console.h"

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

    template <typename T>
    static T valueFromString(const std::string &s) {
        T v;
        std::stringstream ss(s);
        ss >> v;
        if (ss.fail()) {
            throw std::runtime_error("Invalid argument! Value: " + s);
        }
        return v;
    }

    static void invalidValue(const std::string &info);
    static void waitForEnter();
    static bool readArguments(std::map<std::string, Argument> &arguments);
    static size_t fileSize(std::ifstream &file);
    static void setCLI(bool cli);
    static bool isCLIMode();

private:
    static bool isCLI;
};

#ifndef __APPLE__
uint64_t HTONLL(uint64_t x);
uint64_t NTOHLL(uint64_t x);
#endif

#endif  // DEMOAPP_UTILS_H