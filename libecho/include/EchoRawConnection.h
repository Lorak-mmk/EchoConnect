#ifndef ECHOCONNECT_ECHO_RAW_CONNECTION_H
#define ECHOCONNECT_ECHO_RAW_CONNECTION_H

#include <cstdint>
#include <memory>
#include <vector>

#include "IReceiver.h"
#include "ISender.h"

/**
 *
 */
class EchoRawConnection {
public:
    /**
     *
     */
    static EchoRawConnection getBitEchoRawConnection(int winSize, int receiveLoFreq, int receiveHiFreq, int sendLoFreq,
                                                     int sendHiFreq, int magLim);

    /**
     * @brief Sends given byte sequence.
     *
     * Anyone can receive it, and there is no guarantee that received transmission will be correct.
     * @param buffer    Bytes to send.
     */
    void send(const std::vector<uint8_t> &buffer);

    /**
     * @brief Waits for the signal, returns buffer with received bytes.
     * Works by reading fixed-sized windows from the input stream and computing
     * the discrene fourier transform for each one for 2 specific frequencies.
     * If they're both sufficiently low, it registers no signal. Otherwise the
     * larger one gets interpreted as the sent bit. The recording stops after
     * receiving a non-registered bit again. For larger robustness, the
     * function actually reads windows twice as short and discards every other
     * one, because the sender and receiver may be aligned differently.
     * For now, there is no guarantee regarding correctness of transmission.
     * @return  Received bytes.
     */
    int receiveFirst(uint8_t *buffer, int size);

    /**
     *
     */
    int receive(uint8_t *buffer, int size);

private:
    std::unique_ptr<IReceiver> receiver; /**< Receiver used by instance. */
    std::unique_ptr<ISender> sender;     /**< Sender used by instance. */
};

#endif  // ECHOCONNECT_ECHO_RAW_CONNECTION_H
