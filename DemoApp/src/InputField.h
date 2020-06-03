#ifndef DEMOAPP_INPUT_FIELD_H
#define DEMOAPP_INPUT_FIELD_H

#include <string>

#ifdef _WIN32
using ssize_t = long long;
#endif

class InputField {
public:
    InputField() {}
    void clear();
    void add(char c);
    void moveLeft();
    void moveRight();
    std::string getLastNLines(size_t lineWidth, ssize_t n);

private:
    std::string text;
    size_t cursor = 0;
};

#endif  // DEMOAPP_INPUT_FIELD_H