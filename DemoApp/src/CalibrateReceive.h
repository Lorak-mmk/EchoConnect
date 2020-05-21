#ifndef DEMOAPP_CALIBRATE_RECEIVE_H
#define DEMOAPP_CALIBRATE_RECEIVE_H

#include <utility>

#include "AAction.h"
#include "Config.h"

class CalibrateReceive : public AAction {
public:
    CalibrateReceive(std::string title) : AAction(std::move(title)) {
        arguments.insert({recvFreqKey, Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({winSizeKey, Argument("sound window size", ArgumentType::INTEGER)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_CALIBRATE_RECEIVE_H