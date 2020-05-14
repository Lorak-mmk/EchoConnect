#include "SendText.h"

void SendText::setDefaults() {}

ViewPtr SendText::runAction() {
    return parent;
}