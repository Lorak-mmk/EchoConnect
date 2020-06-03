#ifndef DEMOAPP_ARGUMENT_H
#define DEMOAPP_ARGUMENT_H

#include <iostream>
#include <utility>
#include <variant>

#include "Console.h"

enum class ArgumentType { INTEGER, REAL, STRING };

typedef struct Argument {
    std::string title;
    ArgumentType type;
    bool valueSet;
    std::variant<std::string, int, double> value;

    Argument(std::string title, ArgumentType type) : title(std::move(title)), type(type) {
        valueSet = false;
    }

    void print() const {
        switch (type) {
            case ArgumentType::INTEGER:
                std::cout << setFormatting({ConsoleFormat::BOLD}) << " " << title << clearFormatting()
                          << " - integer: ";
                if (valueSet) {
                    std::cout << setFormatting({ConsoleFormat::T_BLUE}) << std::get<int>(value) << "\n"
                              << clearFormatting();
                } else {
                    std::cout << setFormatting({ConsoleFormat::T_RED}) << "not specified\n" << clearFormatting();
                }
                break;

            case ArgumentType::REAL:
                std::cout << setFormatting({ConsoleFormat::BOLD}) << " " << title << clearFormatting()
                          << " - floating point number: ";
                if (valueSet) {
                    std::cout << setFormatting({ConsoleFormat::T_BLUE}) << std::get<double>(value) << "\n"
                              << clearFormatting();
                } else {
                    std::cout << setFormatting({ConsoleFormat::T_RED}) << " not specified\n" << clearFormatting();
                }
                break;

            case ArgumentType::STRING:
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

    std::string typeString() {
        switch (type) {
            case ArgumentType::INTEGER:
                return "Integer";
            case ArgumentType::REAL:
                return "Real";
            case ArgumentType::STRING:
                return "String";
            default:
                return "";
        }
    }

} Argument;

#endif  // DEMOAPP_ARGUMENT_H