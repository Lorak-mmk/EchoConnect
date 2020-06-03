#ifndef DEMOAPP_CONSOLE_H
#define DEMOAPP_CONSOLE_H

enum ConsoleFormat : int {
    BOLD = 1,
    T_BLACK = 30,
    T_RED = 31,
    T_GREEN = 32,
    T_YELLOW = 33,
    T_BLUE = 34,
    T_MAGENTA = 35,
    T_CYAN = 36,
    T_WHITE = 37,
    B_BLACK = 40,
    B_RED = 41,
    B_GREEN = 42,
    B_YELLOW = 43,
    B_BLUE = 44,
    B_MAGENTA = 45,
    B_CYAN = 46,
    B_WHITE = 37
};

size_t getConsoleWidth();
size_t getConsoleHeight();

void setCanon();
void setNoCanon();
void setEcho();
void setNoEcho();

std::string cursorUp(size_t n);
std::string cursorDown(size_t n);
std::string cursorLeft(size_t n);
std::string cursorRight(size_t n);
std::string setCursor(size_t row, size_t column);

std::string clearScreen();
std::string clearLinesBelow();

std::string setFormatting(std::initializer_list<ConsoleFormat> formats);
std::string clearFormatting();

#endif  // DEMOAPP_CONSOLE_H