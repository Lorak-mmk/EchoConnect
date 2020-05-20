#ifndef DEMOAPP_RECEIVE_TEXT_H
#define DEMOAPP_RECEIVE_TEXT_H

#include "AAction.h"

class ReceiveText : public AAction {
public:
    ReceiveText(std::string name, std::string title) : AAction(name, title) {
        arguments.insert({"sendFreq", Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"recvFreq", Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"winSize", Argument("sound window size", ArgumentType::INTEGER)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_RECEIVE_TEXT_H