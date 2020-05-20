#include "EchoRawConnection.h"
#include "EchoRawConnectionImpl.h"

EchoRawConnection *EchoRawConnection::getBitEchoRawConnection(int win_size, int send_lo_freq, int send_hi_freq,
                                                              int recv_lo_freq, int recv_hi_freq, int left_lim,
                                                              int right_lim) {
    return EchoRawConnectionImpl::getBitEchoRawConnection(win_size, send_lo_freq, send_hi_freq, recv_lo_freq,
                                                          recv_hi_freq, left_lim, right_lim);
}

EchoRawConnection *EchoRawConnection::getBitEchoRawConnectionv2(int win_size, int send_freq, int recv_freq, int lim) {
    return EchoRawConnectionImpl::getBitEchoRawConnectionv2(win_size, send_freq, recv_freq, lim);
}
