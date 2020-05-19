#include "AAction.h"
#include "Config.h"
#include "Utils.h"

using namespace std::string_literals;

void AAction::setDefaults() {
    for (Argument &a : arguments) {
        switch (a.type) {
            case INTEGER:
                a.value = getMainConfig()->getValue(0, a.name);
                break;
            case DOUBLE:
                a.value = getMainConfig()->getValue(0.0, a.name);
                break;
            case STRING:
                a.value = getMainConfig()->getValue(""s, a.name);
                break;
            default:
                break;
        }

        a.valueSet = true;
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