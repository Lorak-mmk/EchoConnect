#include <iostream>

#include "views.h"

int main() {
    ViewPtr mainMenu = std::make_shared<Menu>("mainMenu", "Main Menu");
    ViewPtr experimentalMenu = std::make_shared<Menu>("experimentalMenu", "Experimental");
    ViewPtr sendMenu = std::make_shared<Menu>("sendMenu", "Send");
    ViewPtr receiveMenu = std::make_shared<Menu>("receiveMenu", "Receive");
    ViewPtr sendText = std::make_shared<SendText>("sendText", "Send text");
    ViewPtr receiveText = std::make_shared<ReceiveText>("receiveText", "Receive text");
    ViewPtr sendFile = std::make_shared<SendFile>("sendFile", "Send file");
    ViewPtr settings = std::make_shared<Settings>("settings", "Settings");
    ViewPtr help = std::make_shared<Help>("help", "Help");

    mainMenu->addChild(sendMenu);
    sendMenu->setParent(mainMenu);
    mainMenu->addChild(receiveMenu);
    receiveMenu->setParent(mainMenu);
    mainMenu->addChild(experimentalMenu);
    experimentalMenu->setParent(mainMenu);
    mainMenu->addChild(settings);
    settings->setParent(mainMenu);
    mainMenu->addChild(help);
    help->setParent(mainMenu);
    sendMenu->addChild(sendText);
    sendText->setParent(sendMenu);
    sendMenu->addChild(sendFile);
    sendFile->setParent(sendMenu);
    receiveMenu->addChild(receiveText);
    receiveText->setParent(receiveMenu);

    ViewPtr currentView = mainMenu;

    Config *config = getMainConfig();

    for (;;) {
        currentView = currentView->execute();
    }
}