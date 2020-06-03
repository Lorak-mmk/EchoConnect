#include "InputField.h"

#include <algorithm>

void InputField::clear() {
    iterator = text.end();
    cursor = 0;
}

void InputField::add(char c) {
    iterator = text.insert(iterator, c);
    cursor++;
    iterator++;
}

void InputField::moveLeft() {
    if (cursor > 0) {
        cursor--;
        iterator--;
    }
}

void InputField::moveRight() {
    if (cursor < text.size()) {
        cursor++;
        iterator++;
    }
}

std::string InputField::getLastNLines(size_t lineWidth, ssize_t n) {
    if (n < 0) {
        return text;
    }

    size_t fullLines = text.size() / lineWidth, rest = text.size() % lineWidth;
    size_t notFullLines = rest == 0 ? 0 : 1;
    size_t charsToReturn = rest + std::min(fullLines, n - notFullLines);
    size_t start = text.size() - charsToReturn;

    return text.substr(start, charsToReturn);
}