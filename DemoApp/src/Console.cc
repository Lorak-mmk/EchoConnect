#include <termios.h>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#elif defined __unix__
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include "Console.h"

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

void setFlag(tcflag_t flag, bool state) {
    struct termios tios;
    tcgetattr(0, &tios);
    if (state) {
        tios.c_lflag |= flag;
    } else
        tios.c_lflag &= ~flag;
}

tcsetattr(0, TCSAFLUSH, &tios);
}

void setCanon() {
    setFlag(ICANON, true);
}

void setNoCanon() {
    setFlag(ICANON, true);
}
void setEcho() {
    setFlag(ECHO, true);
}

void setNoEcho() {
    setFlag(ECHO, false);
}

std::string cursorUp(size_t n) {
    std::stringstream ss;
    ss << "\033[" << n << "F";
    return ss.str();
}

std::string cursorDown(size_t n) {
    std::stringstream ss;
    ss << "\033[" << n << "E";
    return ss.str();
}

std::string cursorLeft(size_t n) {
    std::stringstream ss;
    ss << "\033[" << n << "D";
    return ss.str();
}

std::string cursorRight(size_t n) {
    std::stringstream ss;
    ss << "\033[" << n << "C";
    return ss.str();
}

std::string setCursor(size_t row, size_t column) {
    std::stringstream ss;
    ss << "\033[" << row << ";" << column << "f";
    return ss.str();
}

std::string clearScreen() {
    return "\033[2J\033[0;0H";
}

std::string clearLinesBelow() {
    std::stringstream ss;
    ss << "\033[" << getConsoleHeight() << "M";
    return ss.str();
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