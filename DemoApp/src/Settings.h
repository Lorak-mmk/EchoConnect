#ifndef DEMOAPP_SETTINGS_H
#define DEMOAPP_SETTINGS_H

#include "AAction.h"

class Settings : public AAction {
public:
    Settings(std::string name, std::string title) : AAction(name, title) {
        arguments.emplace_back(Argument("sendFreq", "dafault sending sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("recvFreq", "default receiving sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("winSize", "default sound window size", ArgumentType::INTEGER));
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SETTINGS_H