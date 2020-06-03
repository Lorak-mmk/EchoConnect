#ifndef DEMOAPP_MENU_H
#define DEMOAPP_MENU_H

#include <utility>

#include "AView.h"

class Menu : public AView {
public:
    Menu(std::string title, std::vector<ViewPtr> children) : AView(std::move(title), std::move(children)) {}

    ViewPtr execute() override;
    void printHelp() override;
    bool executeCLI(const std::string& name, const std::map<std::string, std::string>& args) override;
};

#endif  // DEMOAPP_MENU_H