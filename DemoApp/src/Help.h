#ifndef DEMOAPP_HELP_H
#define DEMOAPP_HELP_H

#include <utility>

#include "AAction.h"

class Help : public AAction {
public:
    Help(std::string name, std::string title) : AAction(std::move(name), std::move(title)) {}

    void printHelp() override;
    bool executeCLI(const std::string &name, const std::map<std::string, std::string> &args) override;

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_HELP_H