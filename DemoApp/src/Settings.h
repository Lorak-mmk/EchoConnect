#ifndef DEMOAPP_SETTINGS_H
#define DEMOAPP_SETTINGS_H

#include "AAction.h"

class Settings : public AAction {
public:
    Settings(std::string name, std::string title) : AAction(name, title) {
        arguments.emplace_back(Argument("default sending sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("default receiving sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("default sound window size", ArgumentType::INTEGER));
    }

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SETTINGS_H