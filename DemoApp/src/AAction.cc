#include "AAction.h"
#include "Config.h"
#include "Utils.h"

using namespace std::string_literals;

void AAction::setDefaults() {
    for (auto &[key, arg] : arguments) {
        switch (arg.type) {
            case ArgumentType::INTEGER:
                arg.value = getMainConfig()->getValue(0, key);
                break;
            case ArgumentType::REAL:
                arg.value = getMainConfig()->getValue(0.0, key);
                break;
            case ArgumentType::STRING:
                arg.value = getMainConfig()->getValue(""s, key);
                break;
            default:
                break;
        }

        arg.valueSet = true;
    }
}

ViewPtr AAction::execute() {
    Utils::clear();
    Utils::printTitle(title);
    setDefaults();

    if (!Utils::readArguments(arguments)) {
        return parent;
    }

    return runAction();
}

void AAction::printHelp() {
    std::cout << "\t" << name << "      ";
    for(auto& opt : this->arguments) {
        std::cout << opt.first <<" (" << opt.second.typeString() << "), ";
    }
    std::cout << "\n";
}

bool AAction::executeCLI(const std::string &name, const std::map<std::string, std::string> &args) {
    if(this->name != name) {
        return false;
    }

    setDefaults();

    for(auto &arg : args) {
        if(!arguments.count(arg.first)) {
            throw std::runtime_error("Invalid argument name: " + arg.first);
        }

        Argument &a = arguments.at(arg.first);
        switch(a.type) {
            case ArgumentType::STRING:
                a.value = Utils::valueFromString<std::string>(arg.second);
                break;
            case ArgumentType::REAL:
                a.value = Utils::valueFromString<double>(arg.second);
                break;
            case ArgumentType::INTEGER:
                a.value = Utils::valueFromString<int>(arg.second);
                break;
        }
    }

    std::cout << "Executing " << this->title << "\n";
    (void)runAction();
    return true;
}
