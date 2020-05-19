#include "Settings.h"
#include "Config.h"
#include "Console.h"
#include "Utils.h"

ViewPtr Settings::runAction() {
    std::cout << " Please wait, we are updating your preferences";

    for (const Argument &a : arguments) {
        switch (a.type) {
            case INTEGER:
                getMainConfig()->setValue(std::get<int>(a.value), a.name);
                break;

            case DOUBLE:
                getMainConfig()->setValue(std::get<double>(a.value), a.name);
                break;

            case STRING:
                getMainConfig()->setValue(std::get<std::string>(a.value), a.name);
                break;

            default:
                break;
        }

        std::cout << ".";
    }

    std::cout << setFormatting({ConsoleFormat::T_YELLOW}) << "\n Done, press enter to go back to the previous view..."
              << clearFormatting();
    Utils::waitForEnter();

    return parent;
}