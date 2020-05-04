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

void EchoRawConnectionImpl::sendStart() {
    sender->start();
}

void EchoRawConnectionImpl::send(uint8_t *buffer, int size) {
    sender->send(buffer, size);
}

void EchoRawConnectionImpl::sendWait() {
    sender->wait();
}

void EchoRawConnectionImpl::receiveStart() {
    receiver->start();
}

int EchoRawConnectionImpl::receive(uint8_t *buffer, int size) {
    return receiver->receive(buffer, size);
}
