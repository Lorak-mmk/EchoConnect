#include "echo.h"
#include <iostream>


void echo::send(const std::vector<uint8_t>& buffer) {
    std::cout << "Sending: ";
    for (auto c : buffer) {
        std::cout << c;
    }
    std::cout << std::endl;
}

std::vector<uint8_t> echo::receive() {
    std::vector<uint8_t> result;
    const size_t LENGTH = 40;
    for (size_t i = 0; i < LENGTH; i++) {
        result.push_back(static_cast<uint8_t>('0' + i));
    }
    return result;
}
