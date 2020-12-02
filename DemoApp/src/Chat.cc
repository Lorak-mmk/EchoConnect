#include "Chat.h"
#include "Config.h"
#include "Console.h"
#include "Utils.h"

#include <cstdio>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

constexpr char NEW_LINE = '\n';

void Chat::drawChat() {
    std::lock_guard<std::mutex> lock(mutex);
    size_t width = getConsoleWidth(), height = getConsoleHeight();
    std::cout << setCursor(4, 0) << clearLinesBelow();
    auto messages = chat->getLastNLines(width - 5, height - 10);
    for (const auto &m : messages) {
        std::cout << " " << m << "\n";
    }
    std::cout << setCursor(height - 3, 0) << setFormatting({ConsoleFormat::T_BLACK, ConsoleFormat::B_WHITE})
              << "To exit chat type \"~end\" and press enter. Type your message below, press enter to send:\n"
              << clearFormatting();
    std::cout << input->getLastNLines(width - 5, 3);
    fflush(stdout);
}

void Chat::sendRecv() {
    std::vector<char> receivedMessage;
    size_t bufforSize = 4096;
    receivedMessage.resize(bufforSize);

    while (run) {
        try {
            auto messageToSend = toSend->popFront();
            if (messageToSend.has_value()) {
                protocol->write(messageToSend.value().data(), messageToSend.value().size());
            }

            size_t messageLength = 0;
            ssize_t readLength = protocol->read(receivedMessage.data(), 1, 1);
            if (readLength > 0) {
                messageLength += readLength;
                while (receivedMessage[messageLength - 1] != '\0') {
                    readLength = protocol->read(receivedMessage.data() + messageLength, 1, 1);
                    if (readLength < 0) {
                        run = false;
                        return;
                    }
                    messageLength += readLength;
                }
                chat->pushBack(std::string(receivedMessage.data(), messageLength));
                drawChat();
            } else if (readLength < 0) {
                run = false;
                return;
            }
        } catch (std::exception &e) {
            run = false;
            return;
        }
    }
}

bool Chat::readInput(const std::string &username) {
    while (run) {
        int c = getchar();
        switch (c) {
            case NEW_LINE: {
                auto message = input->getLastNLines(0, -1);
                input->clear();

                if (message == "~end") {
                    run = false;
                    return true;
                }

                auto messSend = setFormatting({ConsoleFormat::T_CYAN})
                                    .append(username)
                                    .append(": ")
                                    .append(clearFormatting())
                                    .append(message)
                                    .append(1, '\0');
                auto messShow =
                    setFormatting({ConsoleFormat::T_MAGENTA}).append("You: ").append(clearFormatting()).append(message);

                toSend->pushBack(messSend);
                chat->pushBack(messShow);
            } break;
            default:
                input->add((char)c);
                break;
        }

        drawChat();
    }
    return false;
}

ViewPtr Chat::runAction() {
    int winSize = std::get<int>(arguments.find("winSize")->second.value);
    int sendFreq = std::get<int>(arguments.find("sendFreq")->second.value);
    int recvFreq = std::get<int>(arguments.find("recvFreq")->second.value);
    int listen = std::get<int>(arguments.find("listen")->second.value);
    std::string username = std::get<std::string>(arguments.find("username")->second.value);

    run = true;
    input = std::make_unique<InputField>();
    toSend = std::make_unique<ConcurrentBuffer>();
    chat = std::make_unique<ConcurrentBuffer>();
    protocol = std::make_unique<EchoProtocol>(winSize, sendFreq, recvFreq,
                                              (int)getMainConfig()->getLimFor(recvFreq, winSize, 0.0));

    std::cout << setFormatting({ConsoleFormat::T_BLUE})
              << " We are analyzing if there is any ongoing transmission, please wait...\n"
              << clearFormatting();

    try {
        if (listen) {
            protocol->listen();
        } else {
            protocol->connect();
        }
    } catch (std::exception &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Unfortunately we could detect any transmission, press enter to return to the previous view...\n"
                  << clearFormatting();
        Utils::waitForEnter();
        return parent;
    }

    disableCanon();
    disableEcho();

    std::thread audioWorker{&Chat::sendRecv, this};
    audioWorker.detach();

    drawChat();

    std::string message;
    if (readInput(username)) {
        message = setFormatting({ConsoleFormat::T_BLUE}) +
                  " You've decided to finish chatting, press enter to return to the previous view...\n" +
                  clearFormatting();
    } else {
        message = setFormatting({ConsoleFormat::T_RED}) +
                  " An error occured, press enter to return to the previous view...\n" + clearFormatting();
    }

    protocol->close();

    enableCanon();
    enableEcho();

    size_t height = getConsoleHeight();

    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    std::cout << setCursor(height - 3, 0) << clearLinesBelow() << "\n" << message;
    Utils::waitForEnter();

    return parent;
}

void Chat::printHelp() {}

bool Chat::executeCLI(const std::string &name, const std::map<std::string, std::string> &args) {
    (void)name;
    (void)args;
    return false;
}
