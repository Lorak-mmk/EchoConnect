#include "AAction.h"
#include "Utils.h"

ViewPtr AAction::execute() {
    Utils::clear();
    Utils::printTitle(title);
    setDefaults();

    if (!Utils::readArguments(arguments)) {
        return parent;
    }

    return runAction();
}