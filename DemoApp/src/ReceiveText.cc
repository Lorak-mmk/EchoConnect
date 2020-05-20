#include "ReceiveText.h"
#include "Config.h"
#include "Console.h"
#include "EchoProtocol.h"
#include "Utils.h"

ViewPtr ReceiveText::runAction() {
    int winSize = std::get<int>(arguments.find("winSize")->second.value);
    int sendFreq = std::get<int>(arguments.find("sendFreq")->second.value);
    int recvFreq = std::get<int>(arguments.find("recvFreq")->second.value);

    std::vector<char> message;
    size_t bufforSize = 4096;
    message.resize(bufforSize);
    size_t messageLength = 0, readLength = 0;

    EchoProtocol protocol(winSize, sendFreq, recvFreq, getMainConfig()->getLimFor(recvFreq, winSize));

    std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " We are analyzing if there is any ongoing transmission, please wait...\n";

    try {
        protocol.listen();
    } catch (std::exception &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Unfortunately we could detect any transmission, press enter to return to the previous view...\n"
                  << clearFormatting();
        Utils::waitForEnter();
    }

    std::cout << " We have detected an ongoing transmission and started receiving the message, please wait...\n";

    try {
        while ((readLength = protocol.read(message.data() + messageLength, bufforSize - messageLength, 10)) >= 0) {
            messageLength += readLength;
            if (bufforSize - messageLength < (bufforSize >> 1)) {
                bufforSize <<= 1;
                message.resize(bufforSize);
            }
        }
        protocol.close();

    } catch (std::exception &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Unfortunately transmission haven't finished correctly and the message isn't complete.\n"
                  << clearFormatting();
    }

    std::cout << clearFormatting() << setFormatting({ConsoleFormat::BOLD}) << "\n Received message:\n\n"
              << clearFormatting();

    std::cout << message.data();

    std::cout << setFormatting({ConsoleFormat::T_YELLOW}) << "\n\n Press enter to return from this view..."
              << clearFormatting();
    Utils::waitForEnter();

    return parent;
}