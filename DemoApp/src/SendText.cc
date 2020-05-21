#include <limits>

#include "Config.h"
#include "Console.h"
#include "EchoProtocol.h"
#include "SendText.h"
#include "Utils.h"

ViewPtr SendText::runAction() {
    int winSize = std::get<int>(arguments.find("winSize")->second.value);
    int sendFreq = std::get<int>(arguments.find("sendFreq")->second.value);
    int recvFreq = std::get<int>(arguments.find("recvFreq")->second.value);

    EchoProtocol protocol(winSize, sendFreq, recvFreq, (int)getMainConfig()->getLimFor(recvFreq, winSize, 0.0));

    std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " Please enter text you want to send: " << clearFormatting();
    std::string message;
    std::cin >> message;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    try {
        protocol.connect();
    } catch (std::exception &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Unfortunately we couldn't connect with any host willing to receive the message, press enter to "
                     "return to the previous view...\n"
                  << clearFormatting();
        Utils::waitForEnter();
        return parent;
    }

    try {
        protocol.write(message.data(), message.length());
        protocol.close();
    } catch (std::exception &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Unfortunately an error occured while sending the message, press enter to "
                     "return to the previous view...\n"
                  << clearFormatting();
        Utils::waitForEnter();
        return parent;
    }

    std::cout << setFormatting({ConsoleFormat::T_YELLOW})
              << "\n\n Message sent successfuly, press enter to return to the previous view..." << clearFormatting();

    Utils::waitForEnter();

    return parent;
}