#ifndef DEMOAPP_SETTINGS_H
#define DEMOAPP_SETTINGS_H

#include "AAction.h"

class Settings : public AAction {
public:
    Settings(std::string title) : AAction(title) {
        arguments.insert({"sendFreq", Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"recvFreq", Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"winSize", Argument("sound window size", ArgumentType::INTEGER)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SETTINGS_H