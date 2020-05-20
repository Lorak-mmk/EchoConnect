#ifndef DEMOAPP_SENDFILE_H
#define DEMOAPP_SENDFILE_H

#include <utility>

#include "AAction.h"
#include "Config.h"

class SendFile : public AAction {
public:
    explicit SendFile(std::string title) : AAction(std::move(title)) {
        arguments.insert({sendFreqKey, Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({recvFreqKey, Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({winSizeKey, Argument("sound window size", ArgumentType::INTEGER)});
        arguments.insert({"filename", Argument("file to send", ArgumentType::STRING)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SENDFILE_H