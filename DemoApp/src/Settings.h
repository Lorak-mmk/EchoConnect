#ifndef DEMOAPP_SETTINGS_H
#define DEMOAPP_SETTINGS_H

#include "AAction.h"

class Settings : public AAction {
public:
    Settings(std::string name, std::string title) : AAction(name, title) {
        arguments.emplace_back(Argument("sendFreq", "sending sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("recvFreq", "receiving sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("winSize", "sound window size", ArgumentType::INTEGER));
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SETTINGS_H