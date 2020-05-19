#ifndef DEMOAPP_IACTION_H
#define DEMOAPP_IACTION_H

#include <string>
#include <vector>

#include "AView.h"
#include "Argument.h"

class AAction : public AView {
public:
    AAction(std::string name, std::string title) : AView(name, title) {}

    virtual ~AAction() = default;

    ViewPtr execute() override;

protected:
    std::vector<Argument> arguments;
    virtual void setDefaults();
    virtual ViewPtr runAction() = 0;
};

#endif  // DEMOAPP_IACTION_H