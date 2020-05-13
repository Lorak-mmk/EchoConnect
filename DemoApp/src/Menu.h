#ifndef DEMOAPP_MENU_H
#define DEMOAPP_MENU_H

#include "IView.h"

class Menu : public IView {
public:
    Menu(std::string name) : IView(name) {}

    // 	Menu(const Menu &other) = delete;
    // 	Menu(Menu &&other) = default;
    // 	Menu &operator=(const Menu &other) = delete;
    // 	Menu &operator=(Menu &&other) = default;
    // 	virtual ~Menu() = default;

    ViewPtr execute() override;
};

#endif  // DEMOAPP_MENU_H