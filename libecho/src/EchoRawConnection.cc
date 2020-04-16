#include "EchoRawConnection.h"
#include "EchoRawConnectionImpl.h"

EchoRawConnection *EchoRawConnection::getBitEchoRawConnection(int winSize, int receiveLoFreq, int receiveHiFreq,
                                                              int sendLoFreq, int sendHiFreq, int leftLim, int rightLim) {
    return EchoRawConnectionImpl::getBitEchoRawConnection(winSize, receiveLoFreq, receiveHiFreq, sendLoFreq, sendHiFreq,
                                                          leftLim, rightLim);
}
