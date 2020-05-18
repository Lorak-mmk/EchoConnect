#include <iostream>

#include "Console.h"
#include "Menu.h"
#include "Utils.h"

void printOption(int nr, std::string name) {
    std::cout << "\t" << nr << ". " << name << "\n";
}

ViewPtr Menu::execute() {
    Utils::clear();

    Utils::printTitle(title);

    size_t i = 1;
    for (const ViewPtr &childPtr : children) {
        printOption(i, childPtr->getTitle());
        i++;
    }

    printOption(0, "Previous view");

    std::string prompt = setFormatting({ConsoleFormat::T_YELLOW}) + "\n\tGo to: " + clearFormatting();

    while ((i = Utils::readValue<size_t>(prompt)) > children.size()) {
        Utils::invalidValue("\tNot a valid option, press enter to continue...");
        std::cout << cursorUp(3) << clearLinesBelow();
    }

    if (i == 0) {
        if (!parent) {
            exit(0);
        }
        return parent;
    }
    return children[i - 1];
}
