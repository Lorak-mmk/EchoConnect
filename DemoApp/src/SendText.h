#ifndef DEMOAPP_SEND_TEXT_H
#define DEMOAPP_SEND_TEXT_H

#include "AAction.h"

class SendText : public AAction {
public:
    SendText(std::string title) : AAction(title) {
        arguments.insert({"sendFreq", Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"recvFreq", Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"winSize", Argument("sound window size", ArgumentType::INTEGER)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SEND_TEXT_H