#include <sstream>
#ifdef _WIN32
#include <windows.h>
#elif defined __unix__
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include "Console.h"
#include "Utils.h"

size_t getConsoleWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO sbInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sbInfo);
    return sbInfo.dwSize.X;
#elif defined __unix__
    struct winsize size {};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
#endif
}

size_t getConsoleHeight() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO sbInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sbInfo);
    return sbInfo.dwSize.Y;
#elif defined __unix__
    struct winsize size {};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_row;
#endif
}

std::string cursorUp(size_t n) {
    if (!Utils::isCLIMode()) {
        std::stringstream ss;
        ss << "\033[" << n << "F";
        return ss.str();
    }
    return "";
}

std::string cursorDown(size_t n) {
    if (!Utils::isCLIMode()) {
        std::stringstream ss;
        ss << "\033[" << n << "E";
        return ss.str();
    }
    return "";
}

std::string setCursor(size_t row, size_t column) {
    if (!Utils::isCLIMode()) {
        std::stringstream ss;
        ss << "\033[" << row << ";" << column << "f";
        return ss.str();
    }
    return "";
}

std::string clearScreen() {
    if (!Utils::isCLIMode()) {
        return "\033[2J\033[0;0H";
    }
    return "";
}

std::string clearLinesBelow() {
    if (!Utils::isCLIMode()) {
        std::stringstream ss;
        ss << "\033[" << getConsoleHeight() << "M";
        return ss.str();
    }
    return "";
}

std::string setFormatting(std::initializer_list<ConsoleFormat> formats) {
    std::stringstream ss;
    ss << "\033[";
    for (const ConsoleFormat &format : formats) {
        ss << ";" << format;
    }
    ss << "m";
    return ss.str();
}

std::string clearFormatting() {
    return "\033[0m";
}