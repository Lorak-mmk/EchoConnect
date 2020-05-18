#ifndef DEMOAPP_SENDTEXT_H
#define DEMOAPP_SENDTEXT_H

#include "AAction.h"

class SendText : public AAction {
public:
    SendText(std::string name, std::string title) : AAction(name, title) {
        arguments.emplace_back(Argument("sendFreq", 13000));
        arguments.emplace_back(Argument("recvFreq", 14000));
        arguments.emplace_back(Argument("winSize", 50));
    }

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_SENDTEXT_H