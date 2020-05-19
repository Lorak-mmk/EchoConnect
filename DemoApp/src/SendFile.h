#ifndef DEMOAPP_SENDFILE_H
#define DEMOAPP_SENDFILE_H

#include <utility>

#include "AAction.h"

class SendFile : public AAction {
public:
    SendFile(std::string name, std::string title) : AAction(std::move(name), std::move(title)) {
        arguments.emplace_back(Argument("sending sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("receiving sound frequency", ArgumentType::INTEGER));
        arguments.emplace_back(Argument("sound window size", ArgumentType::INTEGER));
    }

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SENDFILE_H