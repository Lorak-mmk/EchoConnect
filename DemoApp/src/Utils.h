#ifndef DEMOAPP_UTILS_H
#define DEMOAPP_UTILS_H

#include <iostream>
#include <limits>
#include <vector>

#include "Argument.h"
#include "Console.h"

class Utils {
public:
    static void clear();
    static void printTitle(std::string title);
    static void printWrapped(const std::string &s);

    template <typename T>
    static T readValue(const std::string &prompt) {
        T v;
        std::cout << prompt;
        std::cin >> v;
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            invalidValue("\tPlease specify a valid value, press enter to continue...");
            std::cout << cursorUp(3) << clearLinesBelow() << prompt;
            std::cin >> v;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return v;
    }
    static void invalidValue(std::string info);
    static void waitForEnter();
    static bool readArguments(std::vector<Argument> &arguments);
};

#endif  // DEMOAPP_UTILS_H