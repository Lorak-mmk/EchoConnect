#include <iostream>

#include "Echo.h"
#include "views.h"

int main(int argc, char *argv[]) {
    Echo::initEcho(argc, argv);

    // clang-format off
    Menu mainMenu{"Main Menu",{
        new Menu{"Send", {
            new SendText{"Send text"},
            new SendFile{"Send file"}
        }},
        new Menu{"Receive", {
            new ReceiveText{"Receive text"},
            new ReceiveFile{"Receive file"}
        }},
		new Chat{"Chat"},
        new Menu{"Calibrate", {
            new CalibrateReceive{"Calibrate receiver"},
            new CalibrateSend{"Send callibration signal"}
        }},
        new Settings{"Settings"},
        new Help{"Help"}
    }};
    // clang-format on

    ViewPtr currentView = &mainMenu;

    while (currentView) {
        currentView = currentView->execute();
    }
}