#include "EchoRawConnection.h"
#include "BitReceiver.h"
#include "BitSender.h"

EchoRawConnection EchoRawConnection::getBitEchoRawConnection(int winSize, int receiveLoFreq, int receiveHiFreq,
                                                             int sendLoFreq, int sendHiFreq, int magLim) {
    EchoRawConnection echorc;
    echorc.receiver = std::make_unique<BitReceiver>(winSize, receiveLoFreq, receiveHiFreq, magLim);
    echorc.sender = std::make_unique<BitSender>(winSize, sendLoFreq, sendHiFreq);
    return echorc;
}

void EchoRawConnection::send(const std::vector<uint8_t> &buffer) {
    sender->send(buffer);
}

int EchoRawConnection::receiveFirst(uint8_t *buffer, int size) {
    return receiver->receiveFirst(buffer, size);
}

int EchoRawConnection::receive(uint8_t *buffer, int size) {
    return receiver->receive(buffer, size);
}
