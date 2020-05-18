#ifndef DEMOAPP_SENDFILE_H
#define DEMOAPP_SENDFILE_H


#include <utility>

#include "AAction.h"

class SendFile : public AAction {
public:
    SendFile(std::string name, std::string title) : AAction(std::move(name), std::move(title)) {
        arguments.emplace_back(Argument("sendFreq", 13000));
        arguments.emplace_back(Argument("recvFreq", 14000));
        arguments.emplace_back(Argument("winSize", 50));
    }

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};


#endif  // DEMOAPP_SENDFILE_H