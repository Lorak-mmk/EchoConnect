#ifndef ECHOCONNECT_ECHO_RAW_CONNECTION_H
#define ECHOCONNECT_ECHO_RAW_CONNECTION_H

#include <cstdint>
#include <memory>
#include <vector>

#include "IReceiver.h"
#include "ISender.h"

/**
 * @brief Public API for raw data through sound sending provided by libecho.
 */
class EchoRawConnection {
public:
    /**
     * @brief Initializes EchoRawConnection object which allows data audio transfer by encoding every bit of information
     * separately.
     *
     * For data transfer, frequencies in which we encode(send) and decode(receive) every bit
     * (respectively loFreq for 0 value and hiFreq for 1 value) are specified.
     * Then each bit is being send for a winSize long period of time.
     * @param winSize       How much of bitrate we want to use to play sound in which we encode bit.
     * @param receiveLoFreq Frequency used to decode bits of value 0 when receving data.
     * @param receiveLoFreq Frequency used to decode bits of value 1 when receiving data.
     * @param sendLoFreq    Frequency used to encode bits of value 0 when sending data.
     * @param sendHiFreq    Frequency used to encode bits of value 1 when sending data.
     * @param magLim        TODO: PawelA
     * @return              Desired EchoRawConnection object.
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
     * @brief Waits for the signal, tries to read @p size bytes, fills @p buffer with received bytes, returns number of
     * read bytes.
     *
     * As the name states it should be called only once (first) per series of data receives.
     * For now, there is no guarantee regarding correctness of transmission.
     * @param buffer    Where to write received bytes.
     * @param size      Size of data to read.
     * @return          Size of read data.
     */
    int receiveFirst(uint8_t *buffer, int size);

    /**
     * @brief Tries to read @p size bytes, fills @p buffer with received bytes, returns number of read bytes.
     *
     * Before series of calling it receiveFirst must be called.
     * For now, there is no guarantee regarding correctness of transmission.
     * @param buffer    Where to write received bytes.
     * @param size      Size of data to read.
     * @return          Size of read data.
     */
    int receive(uint8_t *buffer, int size);

private:
    std::unique_ptr<IReceiver> receiver; /**< Receiver used by instance. */
    std::unique_ptr<ISender> sender;     /**< Sender used by instance. */
};

#endif  // ECHOCONNECT_ECHO_RAW_CONNECTION_H
