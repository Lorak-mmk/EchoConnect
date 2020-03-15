#include "echo.h"
#include <iostream>

void echo::send(uint8_t *buffer, size_t len) {
    std::cout << "Sending: ";
    for (size_t i = 0; i < len; i++) {
        std::cout << (char)buffer[i];
    }
    std::cout << std::endl;
}

uint8_t *echo::receive(size_t *len) {
    const size_t LENGTH = 40;
    *len = LENGTH;
    auto *ret = new uint8_t[LENGTH];
    for (size_t i = 0; i < LENGTH; i++) {
        ret[i] = '0' + i;
    }
    return ret;
}
