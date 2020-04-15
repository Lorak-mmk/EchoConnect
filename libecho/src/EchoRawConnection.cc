#include "EchoRawConnection.h"
#include "BitReceiver.h"
#include "BitSender.h"
#include "EchoRawConnectionImpl.h"

EchoRawConnection *EchoRawConnection::getBitEchoRawConnection(int winSize, int receiveLoFreq, int receiveHiFreq,
                                                              int sendLoFreq, int sendHiFreq, int magLim) {
    return EchoRawConnectionImpl::getBitEchoRawConnection(winSize, receiveLoFreq, receiveHiFreq, sendLoFreq, sendHiFreq,
                                                          magLim);
}
