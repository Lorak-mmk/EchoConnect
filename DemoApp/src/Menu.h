#ifndef DEMOAPP_MENU_H
#define DEMOAPP_MENU_H

#include "AView.h"

class Menu : public AView {
public:
    Menu(std::string name, std::string title) : AView(name, title) {}

    ViewPtr execute() override;
};

#endif  // DEMOAPP_MENU_H