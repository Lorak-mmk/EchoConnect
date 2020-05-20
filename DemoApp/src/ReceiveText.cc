#include "ReceiveText.h"
#include "EchoProtocol.h"
#include "Console.h"
#include "Utils.h"

ViewPtr ReceiveText::runAction() {

    std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " We are analyzing if there is any ongoing transmission, please wait...\n";

    // listen

    std::cout << " We have detected an ongoing transmission and started receiving the message, please wait...\n";

    // receive

    std::cout << clearFormatting() << setFormatting({ConsoleFormat::BOLD}) << "\n Received message:\n\n"
              << clearFormatting();

    std::cout << "message";
    // cout message

    std::cout << setFormatting({ConsoleFormat::T_YELLOW}) << "\n\n Press enter to return from this view..."
              << clearFormatting();
    Utils::waitForEnter();

    return parent;
}