#ifndef DEMOAPP_CALIBRATE_SEND_H
#define DEMOAPP_CALIBRATE_SEND_H

#include <utility>

#include "AAction.h"
#include "Config.h"

class CalibrateSend : public AAction {
public:
    CalibrateSend(std::string title) : AAction(std::move(title)) {
        arguments.insert({sendFreqKey, Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({winSizeKey, Argument("sound window size", ArgumentType::INTEGER)});
    }

protected:
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_CALIBRATE_SEND_H