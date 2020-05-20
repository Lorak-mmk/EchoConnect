#ifndef DEMOAPP_ARGUMENT_H
#define DEMOAPP_ARGUMENT_H

#include <iostream>
#include <variant>

#include "Console.h"

enum ArgumentType { INTEGER, DOUBLE, STRING };

typedef struct Argument {
    std::string title;
    ArgumentType type;
    bool valueSet;
    std::variant<std::string, int, double> value;

    Argument(std::string title, ArgumentType type) : title(title), type(type) {
        valueSet = false;
    }

    void print() const {
        switch (type) {
            case INTEGER:
                std::cout << setFormatting({ConsoleFormat::BOLD}) << " " << title << clearFormatting()
                          << " - integer: ";
                if (valueSet) {
                    std::cout << setFormatting({ConsoleFormat::T_BLUE}) << std::get<int>(value) << "\n"
                              << clearFormatting();
                } else {
                    std::cout << setFormatting({ConsoleFormat::T_RED}) << "not specified\n" << clearFormatting();
                }
                break;

            case DOUBLE:
                std::cout << setFormatting({ConsoleFormat::BOLD}) << " " << title << clearFormatting()
                          << " - floating point number: ";
                if (valueSet) {
                    std::cout << setFormatting({ConsoleFormat::T_BLUE}) << std::get<double>(value) << "\n"
                              << clearFormatting();
                } else {
                    std::cout << setFormatting({ConsoleFormat::T_RED}) << " not specified\n" << clearFormatting();
                }
                break;

            case STRING:
                std::cout << setFormatting({ConsoleFormat::BOLD}) << " " << title << clearFormatting() << " - string: ";
                if (valueSet) {
                    std::cout << setFormatting({ConsoleFormat::T_BLUE}) << std::get<std::string>(value) << "\n"
                              << clearFormatting();
                } else {
                    std::cout << setFormatting({ConsoleFormat::T_RED}) << " not specified\n" << clearFormatting();
                }
                break;
            default:
                break;
        }
    }

} Argument;

#endif  // DEMOAPP_ARGUMENT_H