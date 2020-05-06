#ifndef ECHOCONNECT_ECHO_RAW_CONNECTION_IMPL_H
#define ECHOCONNECT_ECHO_RAW_CONNECTION_IMPL_H

#include "EchoRawConnection.h"
#include "IReceiver.h"
#include "ISender.h"

/**
 * @brief Implementation of EchoRawConnection interface.
 */
class EchoRawConnectionImpl : public EchoRawConnection {
public:
    /**
     * @brief @see EchoRawConnection::getBitEchoRawConnection
     */

    static EchoRawConnectionImpl *getBitEchoRawConnection(int win_size, int send_freq, int recv_freq, int lim);

    /**
     * @brief @see EchoRawConnection::start
     */
    void sendStart() override;

    /**
     * @brief @see EchoRawConnection::send
     */
    void send(uint8_t *buffer, int size) override;

    /**
     * @brief @see EchoRawConnection::wait
     */
    void sendWait() override;

    /**
     * @brief @see EchoRawConnection::receiveFirst
     */
    void receiveStart() override;

    /**
     * @brief @see EchoRawConnection::receive
     */
    int receive(uint8_t *buffer, int size) override;

private:
    std::unique_ptr<IReceiver> receiver; /**< Receiver used by instance. */
    std::unique_ptr<ISender> sender;     /**< Sender used by instance. */
};

#endif  // ECHOCONNECT_ECHO_RAW_CONNECTION_IMPL_H
