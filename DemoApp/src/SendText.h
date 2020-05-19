#ifndef DEMOAPP_SEND_TEXT_H
#define DEMOAPP_SEND_TEXT_H

#include "AAction.h"

class SendText : public AAction {
public:
    SendText(std::string name, std::string title) : AAction(name, title) {
        arguments.emplace_back(Argument("sending sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("receiving sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("sound window size", ArgumentType::INTEGER));
    }

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SEND_TEXT_H