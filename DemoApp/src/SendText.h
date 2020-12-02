#ifndef DEMOAPP_SEND_TEXT_H
#define DEMOAPP_SEND_TEXT_H

#include <utility>

#include "AAction.h"

class SendText : public AAction {
public:
    SendText(std::string name, std::string title) : AAction(std::move(name), std::move(title)) {
        arguments.insert({"sendFreq", Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"recvFreq", Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"winSize", Argument("sound window size", ArgumentType::INTEGER)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SEND_TEXT_H