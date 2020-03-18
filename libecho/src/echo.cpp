#include <iostream>
#include <QCoreApplication>
#include "echo.h"
#include "AudioOutput.h"



void echo::initQT(int argc, char* argv[]) {
    static auto *app = new QCoreApplication(argc, argv);
}

void echo::send(const std::vector<char>& buffer) {
    static QThread eventThread;
    eventThread.start();

    AudioOutput output{&eventThread};
    auto device = output.start();

    QByteArray qbuffer(buffer.data(), buffer.size());
    device->write(qbuffer);

    eventThread.quit();
}

std::vector<uint8_t> echo::receive() {
    std::vector<uint8_t> result;
    const size_t LENGTH = 40;
    for (size_t i = 0; i < LENGTH; i++) {
        result.push_back(static_cast<uint8_t>('0' + i));
    }
    return result;
}
