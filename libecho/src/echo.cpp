#include <chrono>
#include <thread>
#include <QCoreApplication>
#include <QDebug>
#include <QtCore/QBuffer>
#include "echo.h"
#include "AudioOutput.h"

void echo::send(const std::vector<char>& buffer) {
    QByteArray bufArray(buffer.data(), buffer.size());
    auto *qbuf = new QBuffer(&bufArray);
    qbuf->open(QIODevice::ReadOnly);

    QEventLoop loop;

    AudioOutput output{};
    output.start(qbuf);

    qWarning() << "Message size: " << bufArray.size();
    qWarning() << "Buffer size" << qbuf->size();
    qWarning() << "Error: " << output.getOutput()->error();
    qWarning() << "Elapsed/processed" << output.getOutput()->elapsedUSecs() << " " << output.getOutput()->processedUSecs();

    loop.exec();
}

std::vector<uint8_t> echo::receive() {
    std::vector<uint8_t> result;
    const size_t LENGTH = 40;
    for (size_t i = 0; i < LENGTH; i++) {
        result.push_back(static_cast<uint8_t>('0' + i));
    }
    return result;
}
