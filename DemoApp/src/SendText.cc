#include "SendText.h"
#include "Config.h"
#include "Console.h"
#include "EchoProtocol.h"
#include "Utils.h"

ViewPtr SendText::runAction() {
    int winSize = std::get<int>(arguments.find("winSize")->second.value);
    int sendFreq = std::get<int>(arguments.find("sendFreq")->second.value);
    int recvFreq = std::get<int>(arguments.find("recvFreq")->second.value);

    EchoProtocol protocol(winSize, sendFreq, recvFreq, getMainConfig()->getLimFor(recvFreq, winSize, 0.0));

    std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " Please enter text you want to send: ";

    try {
        protocol.connect();
    } catch (std::exception &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Unfortunately we couldn't connect with any host willing to receive message, press enter to "
                     "return to the previous view...\n"
                  << clearFormatting();
        Utils::waitForEnter();
    }

    try {
        protocol.write("abcdefghijkl", 12);
        protocol.close();
    } catch (std::exception &e) {
        puts(e.what());
    }

    std::cout << setFormatting({ConsoleFormat::T_YELLOW})
              << "\n\n  Message sent successfuly, press enter to return to the previous view..." << clearFormatting();
    Utils::waitForEnter();

    return parent;
}