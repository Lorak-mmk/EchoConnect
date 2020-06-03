#include "Chat.h"
#include "ConcurrentBuffer.h"
#include "Config.h"
#include "Console.h"
#include "EchoProtocol.h"
#include "InputField.h"
#include "Utils.h"

#include <stdio.h>
#include <mutex>
#include <thread>

constexpr char KEY_UP = 72;
constexpr char KEY_DOWN = 80;
constexpr char KEY_LEFT = 75;
constexpr char KEY_RIGHT = 77;
constexpr char NEW_LINE = '\n';

std::mutex mutex;

struct StateVariables {
    std::string user;
    EchoProtocol *protocol;
    InputField *input;
    ConcurrentBuffer *toSend, *chat;
    bool run;
};

void drawChat(StateVariables *vars) {
    std::lock_guard<std::mutex> lock(mutex);
    size_t width = getConsoleWidth(), height = getConsoleHeight();
    std::cout << setCursor(4, 0) << clearLinesBelow();
    auto messages = vars->chat->getLastNLines(width - 5, height - 10);
    for (const auto &m : messages) {
        std::cout << " " << m << "\n";
    }
    std::cout << setCursor(height - 3, 0) << setFormatting({ConsoleFormat::T_BLACK, ConsoleFormat::B_WHITE})
              << "To exit chat type \"~end\" and press enter. Type your message below, press enter to send:\n"
              << clearFormatting();
    std::cout << vars->input->getLastNLines(width - 5, 3);
}

void sendRecv(StateVariables *vars) {
    std::vector<char> receivedMessage;
    size_t bufforSize = 4096;
    receivedMessage.resize(bufforSize);

    while (vars->run) {
        try {
            auto messageToSend = vars->toSend->popFront();
            if (messageToSend.has_value()) {
                vars->protocol->write(messageToSend.value().data(), messageToSend.value().size());
            }

            size_t messageLength = 0;
            ssize_t readLength = vars->protocol->read(receivedMessage.data(), 1, 1);
            if (readLength > 0) {
                messageLength += readLength;
                while (receivedMessage[messageLength - 1] != '\0') {
                    readLength = vars->protocol->read(receivedMessage.data() + messageLength, 1, 1);
                    if (readLength < 0) {
                        vars->run = false;
                        return;
                    }
                    messageLength += readLength;
                }
                vars->chat->pushBack(std::string(receivedMessage.data(), messageLength));
                drawChat(vars);
            } else if (readLength < 0) {
                vars->run = false;
                return;
            }
        } catch (std::exception &e) {
            vars->run = false;
            return;
        }
    }
}

bool readInput(StateVariables *vars) {
    while (vars->run) {
        char c = getchar();
        switch (c) {
            case KEY_UP:
                break;
            case KEY_DOWN:
                break;
            case KEY_LEFT:
                vars->input->moveLeft();
                break;
            case KEY_RIGHT:
                vars->input->moveRight();
                break;
            case NEW_LINE: {
                auto message = vars->input->getLastNLines(0, -1);
                vars->input->clear();

                if (message == "~end") {
                    vars->run = false;
                    return true;
                }

                vars->toSend->pushBack(setFormatting({ConsoleFormat::T_CYAN}) + vars->user + ": " + clearFormatting() +
                                       message + '\0');
                vars->chat->pushBack(setFormatting({ConsoleFormat::T_MAGENTA}) + "You: " + clearFormatting() + message +
                                     '\0');
            } break;
            default:
                vars->input->add(c);
                break;
        }

        drawChat(vars);
    }
    return false;
}

ViewPtr Chat::runAction() {
    int winSize = std::get<int>(arguments.find("winSize")->second.value);
    int sendFreq = std::get<int>(arguments.find("sendFreq")->second.value);
    int recvFreq = std::get<int>(arguments.find("recvFreq")->second.value);
    int listen = std::get<int>(arguments.find("listen")->second.value);
    std::string username = std::get<std::string>(arguments.find("username")->second.value);

    InputField i;
    ConcurrentBuffer s, c;
    EchoProtocol p(winSize, sendFreq, recvFreq, (int)getMainConfig()->getLimFor(recvFreq, winSize, 0.0));
    StateVariables vars = {.user = username, .protocol = &p, .input = &i, .toSend = &s, .chat = &c, .run = true};

    std::cout << setFormatting({ConsoleFormat::T_BLUE})
              << " We are analyzing if there is any ongoing transmission, please wait...\n"
              << clearFormatting();

    try {
        if (listen) {
            p.listen();
        } else {
            p.connect();
        }
    } catch (std::exception &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Unfortunately we could detect any transmission, press enter to return to the previous view...\n"
                  << clearFormatting();
        Utils::waitForEnter();
        return parent;
    }

    setNoCanon();
    setNoEcho();

	StateVariables *v = &vars;
    std::thread audioWorker{&sendRecv, v};

    drawChat(&vars);

    std::string message;
    if (readInput(&vars)) {
        message = setFormatting({ConsoleFormat::T_BLUE}) +
                  " You've decided to finish chatting, press enter to return to the previous view...\n" +
                  clearFormatting();
    } else {
        message = setFormatting({ConsoleFormat::T_RED}) +
                  " An error occured, press enter to return to the previous view...\n" + clearFormatting();
    }

	audioWorker.join();
    p.close();

    setCanon();
    setEcho();

    size_t height = getConsoleHeight();
    std::cout << setCursor(height - 3, 0) << clearLinesBelow() << "\n" << message;
    Utils::waitForEnter();

    return parent;
}