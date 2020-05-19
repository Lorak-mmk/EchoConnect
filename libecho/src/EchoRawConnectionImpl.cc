#include "EchoRawConnectionImpl.h"
#include "BitReceiver.h"
#include "BitReceiverv2.h"
#include "BitSender.h"
#include "BitSenderv2.h"

EchoRawConnectionImpl *EchoRawConnectionImpl::getBitEchoRawConnection(int win_size, int send_lo_freq, int send_hi_freq,
                                                                      int recv_lo_freq, int recv_hi_freq, int left_lim,
                                                                      int right_lim) {
    auto *echorc = new EchoRawConnectionImpl;
    echorc->sender = std::make_unique<BitSender>(win_size, send_hi_freq, send_lo_freq);
    echorc->receiver = std::make_unique<BitReceiver>(win_size, recv_lo_freq, recv_hi_freq, left_lim, right_lim);

    return echorc;
}

EchoRawConnectionImpl *EchoRawConnectionImpl::getBitEchoRawConnectionv2(int win_size, int send_freq, int recv_freq,
                                                                        int lim) {
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

void EchoRawConnectionImpl::receiveStart(std::chrono::duration<double> timeout) {
    receiver->start(timeout);
}

int EchoRawConnectionImpl::receive(uint8_t *buffer, int size) {
    return receiver->receive(buffer, size);
}
