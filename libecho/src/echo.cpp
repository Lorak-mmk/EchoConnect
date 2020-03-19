#include <chrono>
#include <thread>
#include <QCoreApplication>
#include <QDebug>
#include <QtCore/QBuffer>
#include "echo.h"
#include "AudioOutput.h"

void echo::send(const std::vector<char>& buffer) {
    static AudioOutput audio;

    audio.enqueueData(buffer.data(), buffer.size());
    std::this_thread::sleep_for(std::chrono::seconds(20));
}

std::vector<uint8_t> echo::receive() {
    std::vector<uint8_t> result;
    const size_t LENGTH = 40;
    for (size_t i = 0; i < LENGTH; i++) {
        result.push_back(static_cast<uint8_t>('0' + i));
    }
    return result;
}
