#include <iostream>

#include "Utils.h"

void Utils::clear() {
    std::cout << "\033c";
}

bool Utils::readArguments(std::vector<Argument> &arguments) {
    return true;
}