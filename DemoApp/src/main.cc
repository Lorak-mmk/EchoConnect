#include "Menu.h"
#include "SendText.h"
#include "Config.h"

#include <memory>

int main() {
    ViewPtr mainMenu = std::make_shared<Menu>("mainMenu");
    ViewPtr experimentalMenu = std::make_shared<Menu>("experimentalMenu");
    ViewPtr sendMenu = std::make_shared<Menu>("sendMenu");
    ViewPtr sendText = std::make_shared<SendText>("sendText");

    mainMenu->addChild(experimentalMenu);
    experimentalMenu->setParent(mainMenu);
    mainMenu->addChild(sendMenu);
    sendMenu->setParent(mainMenu);
    sendMenu->addChild(sendText);
    sendText->setParent(sendMenu);

    ViewPtr currentView = mainMenu;

    Config *config = getMainConfig();

    for (;;) {
        currentView = currentView->execute();
    }
}