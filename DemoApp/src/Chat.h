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
    Chat(std::string title) : AAction(title) {
        arguments.insert({"sendFreq", Argument("sending sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"recvFreq", Argument("receiving sound frequency", ArgumentType::INTEGER)});
        arguments.insert({"winSize", Argument("sound window size", ArgumentType::INTEGER)});
        arguments.insert({"listen", Argument("who initiates connection (0: we connect, other side listens / any other "
                                             "number: we listen, other side connects)",
                                             ArgumentType::INTEGER)});
        arguments.insert({"username", Argument("username", ArgumentType::STRING)});
    }

protected:
    ViewPtr runAction() override;

private:
    void sendRecv();
    void drawChat();
    bool readInput(std::string username);

private:
    std::mutex mutex;
    std::unique_ptr<EchoProtocol> protocol;
    std::unique_ptr<InputField> input;
    std::unique_ptr<ConcurrentBuffer> toSend, chat;
    bool run;
};

#endif  // DEMOAPP_CHAT_H