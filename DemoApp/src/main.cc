#include <iostream>

#include "views.h"

int main() {
    // clang-format off
    Menu mainMenu{"Main Menu",{
        new Menu{"Send", {
            new SendText{"Send text"},
            new SendFile{"Send file"}
        }},
        new Menu{"Receive", {
            new ReceiveText{"Receive text"}
        }},
        new Menu{"Experimental", {}},
        new Settings{"Settings"},
        new Help{"Help"}
    }};
    // clang-format on

    ViewPtr currentView = &mainMenu;

    while (currentView) {
        currentView = currentView->execute();
    }
}