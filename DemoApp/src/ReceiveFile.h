#ifndef DEMOAPP_RECEIVEFILE_H
#define DEMOAPP_RECEIVEFILE_H

#include <utility>

#include "AAction.h"
#include "Config.h"

class ReceiveFile : public AAction {
public:
    explicit ReceiveFile(std::string name, std::string title) : AAction(std::move(name), std::move(title)) {
        arguments.insert({sendFreqKey, Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({recvFreqKey, Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({winSizeKey, Argument("sound window size", ArgumentType::INTEGER)});
        arguments.insert({"filename", Argument("file to send", ArgumentType::STRING)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_RECEIVEFILE_H