#ifndef DEMOAPP_SEND_TEXT_H
#define DEMOAPP_SEND_TEXT_H

#include "AAction.h"

class SendText : public AAction {
public:
    SendText(std::string name, std::string title) : AAction(name, title) {
        arguments.emplace_back(Argument("sendFreq", "sending sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("recvFreq", "receiving sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("winSize", "sound window size", ArgumentType::INTEGER));
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SEND_TEXT_H