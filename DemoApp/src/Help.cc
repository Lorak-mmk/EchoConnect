#include <iostream>

#include "Console.h"
#include "Help.h"
#include "Utils.h"

void Help::setDefaults() {}

ViewPtr Help::runAction() {
    std::cout
        << " To navigate in this application you simply need to enter proper value and click enter to confirm action. "
           " For example entered value may symbolize position in menu or value which you want to assing to a "
           "variable.\n"
           " We encourage you to explore our app and experiment with various functionalities, have fun ^~^\n\n"
        << setFormatting({ConsoleFormat::T_YELLOW}) << " To go back to main menu just press enter."
        << clearFormatting();

    Utils::waitForEnter();
    return parent;
}

void Help::printHelp() {}

bool Help::executeCLI(const std::string &name, const std::map<std::string, std::string> &args) {
    (void)name;
    (void)args;
    return false;
}
