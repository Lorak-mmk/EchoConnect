#include "EchoRawConnection.h"
#include "BitSender.h"
#include "QTInitializer.h"

#include <QCoreApplication>
#include <QDebug>

static constexpr int windowSize = 300;
static constexpr int loFreq = 14500;
static constexpr int hiFreq = 15000;
static constexpr double magLimitBegin = 0.2;  // 300.0 / windowSize;
static constexpr double magLimitEnd = 0.04;

EchoRawConnection::EchoRawConnection() {
    sender = std::make_unique<BitSender>(windowSize, loFreq, hiFreq);
}

void EchoRawConnection::initEcho(int a_argc, char **a_argv) {
    static QTInitializer init{a_argc, a_argv};
}

void EchoRawConnection::send(const std::vector<uint8_t> &buffer) {
    sender.send(buffer);
}

std::vector<uint8_t> EchoRawConnection::receive() {}
