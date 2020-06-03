#include "Settings.h"
#include "Config.h"
#include "Console.h"
#include "Utils.h"

ViewPtr Settings::runAction() {
    std::cout << " Please wait, we are updating your preferences";

    for (auto &[key, arg] : arguments) {
        switch (arg.type) {
            case ArgumentType::INTEGER:
                getMainConfig()->setValue(std::get<int>(arg.value), key);
                break;

            case ArgumentType::REAL:
                getMainConfig()->setValue(std::get<double>(arg.value), key);
                break;

            case ArgumentType::STRING:
                getMainConfig()->setValue(std::get<std::string>(arg.value), key);
                break;

            default:
                break;
        }

        std::cout << ".";
    }

    if (!Utils::isCLIMode()) {
        std::cout << setFormatting({ConsoleFormat::T_YELLOW})
                  << "\n Done, press enter to go back to the previous view..." << clearFormatting();
        Utils::waitForEnter();
    }

    return parent;
}