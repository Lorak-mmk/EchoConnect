#include "EchoRawConnection.h"
#include "EchoRawConnectionImpl.h"

EchoRawConnection *EchoRawConnection::getBitEchoRawConnection(int win_size, int send_freq, int recv_freq, int lim) {
    return EchoRawConnectionImpl::getBitEchoRawConnection(win_size, send_freq, recv_freq, lim);
}
