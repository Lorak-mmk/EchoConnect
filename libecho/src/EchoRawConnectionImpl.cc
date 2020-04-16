#include "EchoRawConnectionImpl.h"
#include "BitReceiver.h"
#include "BitSender.h"

EchoRawConnectionImpl *EchoRawConnectionImpl::getBitEchoRawConnection(int winSize, int receiveLoFreq, int receiveHiFreq,
                                                                      int sendLoFreq, int sendHiFreq, int leftLim,
                                                                      int rightLim) {
    auto *echorc = new EchoRawConnectionImpl;
    echorc->receiver = std::make_unique<BitReceiver>(winSize, receiveLoFreq, receiveHiFreq, leftLim, rightLim);
    echorc->sender = std::make_unique<BitSender>(winSize, sendLoFreq, sendHiFreq);
    return echorc;
}

void EchoRawConnectionImpl::send(const std::vector<uint8_t> &buffer) {
    sender->send(buffer);
}

void EchoRawConnectionImpl::sendBlocking(const std::vector<uint8_t> &buffer) {
    sender->sendBlocking(buffer);
}

void EchoRawConnectionImpl::sendWait() {
    sender->sendWait();
}

int EchoRawConnectionImpl::receiveFirst(uint8_t *buffer, int size) {
    return receiver->receiveFirst(buffer, size);
}

int EchoRawConnectionImpl::receive(uint8_t *buffer, int size) {
    return receiver->receive(buffer, size);
}
