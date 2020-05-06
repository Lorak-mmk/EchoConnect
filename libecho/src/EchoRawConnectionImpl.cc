#include "EchoRawConnectionImpl.h"
#include "BitReceiverv2.h"
#include "BitSenderv2.h"

EchoRawConnectionImpl *EchoRawConnectionImpl::getBitEchoRawConnection(int win_size, int send_freq, int recv_freq, int lim) {
    auto *echorc = new EchoRawConnectionImpl;
    echorc->sender = std::make_unique<BitSenderv2>(win_size, send_freq);
    echorc->receiver = std::make_unique<BitReceiverv2>(win_size, recv_freq, lim);
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
