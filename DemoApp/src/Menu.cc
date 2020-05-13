#include <iostream>

#include "Menu.h"
#include "Utils.h"

ViewPtr Menu::execute() {
    Utils::clear();
    std::cout << "Choose what you want to do:\n";
    std::cout << "0. Return to the previous view\n";

    size_t i = 1;
    for (ViewPtr childPtr : children) {
        std::cout << i << ". " << childPtr->getName() << "\n";
        i++;
    }

    std::cin >> i;
    if (i == 0) {
        return parent;
    }
    return children[i - 1];
}
