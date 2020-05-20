#include <iostream>

#include "views.h"

int main() {
    // clang-format off
    Menu mainMenu{"mainMenu","Main Menu",{
        new Menu{"sendMenu", "Send", {
            new SendText{"sendText", "Send text"},
            new SendFile{"sendFile", "Send file"}
        }},
        new Menu{"receiveMenu", "Receive", {
            new ReceiveText{"receiveText", "Receive text"}
        }},
        new Menu{"experimentalMenu", "Experimental", {}},
        new Settings{"settings", "Settings"},
        new Help{"help", "Help"}
    }};
    // clang-format on

    ViewPtr currentView = &mainMenu;

    while (currentView) {
        currentView = currentView->execute();
    }
}