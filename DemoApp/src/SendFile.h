#ifndef DEMOAPP_SENDFILE_H
#define DEMOAPP_SENDFILE_H

#include <utility>

#include "AAction.h"

class SendFile : public AAction {
public:
    SendFile(std::string title) : AAction(std::move(title)) {
        arguments.insert({"sendFreq", Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"recvFreq", Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"winSize", Argument("sound window size", ArgumentType::INTEGER)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SENDFILE_H