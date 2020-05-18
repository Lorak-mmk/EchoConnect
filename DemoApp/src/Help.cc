#include <iostream>

#include "Console.h"
#include "Help.h"
#include "Utils.h"

void Help::setDefaults() {}

ViewPtr Help::runAction() {
    std::cout
        << "\tTo navigate in this application you simply need to enter proper value and click enter to confirm action. "
           "For example entered value may symbolize position in menu or value which you want to assing to a variable.\n"
           "\tWe encourage you to explore our app and experiment with various functionalities, have fun ^~^\n\n"
        << setFormatting({ConsoleFormat::T_YELLOW}) << "\tTo go back to main menu just press enter."
        << clearFormatting();

    Utils::waitForEnter();
    return parent;
}