#include "AAction.h"
#include "Config.h"
#include "Utils.h"

using namespace std::string_literals;

void AAction::setDefaults() {
    for (auto &[key, arg] : arguments) {
        switch (arg.type) {
            case INTEGER:
                arg.value = getMainConfig()->getValue(0, key);
                break;
            case DOUBLE:
                arg.value = getMainConfig()->getValue(0.0, key);
                break;
            case STRING:
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