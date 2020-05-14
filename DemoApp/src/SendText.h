#ifndef DEMOAPP_SENDTEXT_H
#define DEMOAPP_SENDTEXT_H

#include "IAction.h"

class SendText : public IAction {
public:
    SendText(std::string name) : IAction(name) {
        arguments.emplace_back(Argument("sendFreq", 13000));
        arguments.emplace_back(Argument("recvFreq", 14000));
        arguments.emplace_back(Argument("winSize", 50));
    }

    // 	SendText(const SendText &other) = delete;
    // 	SendText(SendText &&other) = default;
    // 	SendText &operator=(const SendText &other) = delete;
    // 	SendText &operator=(SendText &&other) = default;

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SENDTEXT_H