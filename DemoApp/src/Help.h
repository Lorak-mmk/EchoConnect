#ifndef DEMOAPP_HELP_H
#define DEMOAPP_HELP_H

#include "AAction.h"

class Help : public AAction {
public:
    Help(std::string name, std::string title) : AAction(name, title) {}

protected:
    void setDefaults() override;
    ViewPtr runAction() override;
};

#endif  // DEMOAPP_HELP_H