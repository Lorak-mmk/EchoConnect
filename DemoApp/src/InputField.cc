#include "InputField.h"

#include <algorithm>

void InputField::clear() {
    text = "";
    cursor = 0;
}

void InputField::add(char c) {
    text.insert(text.begin() + cursor, c);
    cursor++;
}

void InputField::moveLeft() {
    if (cursor > 0) {
        cursor--;
    }
}

void InputField::moveRight() {
    if (cursor < text.size()) {
        cursor++;
    }
}

std::string InputField::getLastNLines(size_t lineWidth, ssize_t n) {
    if (n < 0) {
        return text;
    }

    size_t fullLines = text.size() / lineWidth;
    size_t rest = text.size() % lineWidth;
    size_t charsToReturn = rest + std::min(fullLines, (size_t)n - 1) * lineWidth;
    size_t start = text.size() - charsToReturn;

    return text.substr(start, charsToReturn);
}
