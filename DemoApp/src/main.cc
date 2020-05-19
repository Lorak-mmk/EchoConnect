#include <iostream>
#include "Config.h"
#include "views.h"

using namespace std::string_literals;

int main() {
    ViewPtr mainMenu = std::make_shared<Menu>("mainMenu", "Main Menu");
    ViewPtr experimentalMenu = std::make_shared<Menu>("experimentalMenu", "Experimental");
    ViewPtr sendMenu = std::make_shared<Menu>("sendMenu", "Send");
    ViewPtr sendText = std::make_shared<SendText>("sendText", "Send text");
    ViewPtr help = std::make_shared<Help>("help", "Help");

    mainMenu->addChild(sendMenu);
    sendMenu->setParent(mainMenu);
    mainMenu->addChild(experimentalMenu);
    experimentalMenu->setParent(mainMenu);
    mainMenu->addChild(help);
    help->setParent(mainMenu);
    sendMenu->addChild(sendText);
    sendText->setParent(sendMenu);

    ViewPtr currentView = mainMenu;

    Config *config = getMainConfig();

    for (;;) {
        currentView = currentView->execute();
    }
}