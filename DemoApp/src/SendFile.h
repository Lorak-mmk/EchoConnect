#ifndef DEMOAPP_SENDFILE_H
#define DEMOAPP_SENDFILE_H


#include "IAction.h"

class SendFile : public IAction {
public:
    SendFile(std::string name) : IAction(name) {
        arguments.emplace_back(Argument("sendFreq", 13000));
        arguments.emplace_back(Argument("recvFreq", 14000));
        arguments.emplace_back(Argument("winSize", 50));
    }

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};


#endif  // DEMOAPP_SENDFILE_H