#ifndef DEMOAPP_IACTION_H
#define DEMOAPP_IACTION_H

#include <map>
#include <string>
#include <utility>

#include "AView.h"
#include "Argument.h"

class AAction : public AView {
public:
    AAction(std::string name, std::string title) : AView(std::move(name), std::move(title), {}) {}

    ~AAction() override = default;

    ViewPtr execute() override;

protected:
    std::map<std::string, Argument> arguments;
    virtual void setDefaults();
    virtual ViewPtr runAction() = 0;
};

#endif  // DEMOAPP_IACTION_H