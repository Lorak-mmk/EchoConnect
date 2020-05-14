#ifndef DEMOAPP_IACTION_H
#define DEMOAPP_IACTION_H

#include <string>
#include <vector>

#include "Argument.h"
#include "IView.h"

class IAction : public IView {
public:
    IAction(std::string name) : IView(name) {}

    // 	IAction(const IAction &other) = delete;
    // 	IAction(IAction &&other) = default;
    // 	IAction &operator=(const IAction &other) = delete;
    // 	IAction &operator=(IAction &&other) = default;

    virtual ~IAction() = default;

    ViewPtr execute() override;

protected:
    std::vector<Argument> arguments;
    virtual void setDefaults() = 0;
    virtual ViewPtr runAction() = 0;
};

#endif  // DEMOAPP_IACTION_H