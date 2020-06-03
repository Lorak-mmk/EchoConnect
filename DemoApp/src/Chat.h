#ifndef DEMOAPP_CHAT_H
#define DEMOAPP_CHAT_H

#include "AAction.h"
#include "ConcurrentBuffer.h"
#include "EchoProtocol.h"
#include "InputField.h"

#include <mutex>
#include <thread>

class Chat : public AAction {
public:
    Chat(std::string name, std::string title) : AAction(std::move(name), std::move(title)) {
        arguments.insert({"sendFreq", Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"recvFreq", Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"winSize", Argument("sound window size", ArgumentType::INTEGER)});
        arguments.insert({"listen", Argument("who initiates connection (0: we connect, any other number: we listen)",
                                             ArgumentType::INTEGER)});
        arguments.insert({"username", Argument("username", ArgumentType::STRING)});
    }

    void printHelp() override;
    bool executeCLI(const std::string &name, const std::map<std::string, std::string> &args) override;

protected:
    ViewPtr runAction() override;

private:
    void sendRecv();
    void drawChat();
    bool readInput(const std::string &username);

private:
    std::mutex mutex;
    std::unique_ptr<EchoProtocol> protocol;
    std::unique_ptr<InputField> input;
    std::unique_ptr<ConcurrentBuffer> toSend, chat;
    bool run;
};

#endif  // DEMOAPP_CHAT_H
