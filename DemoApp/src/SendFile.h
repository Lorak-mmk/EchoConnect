#ifndef DEMOAPP_SENDFILE_H
#define DEMOAPP_SENDFILE_H

#include <utility>

#include "AAction.h"

class SendFile : public AAction {
public:
    SendFile(std::string name, std::string title) : AAction(std::move(name), std::move(title)) {
        arguments.emplace_back(Argument("sendFreq", "sending sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("recvFreq", "receiving sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("winSize", "sound window size", ArgumentType::INTEGER));
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SENDFILE_H