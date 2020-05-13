#include "IAction.h"
#include "Utils.h"

ViewPtr IAction::execute() {
    setDefaults();
    if (!Utils::readArguments(arguments)) {
        return parent;
    }

    return runAction();
}