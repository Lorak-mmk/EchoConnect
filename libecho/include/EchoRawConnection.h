#ifndef ECHOCONNECT_ECHO_RAW_CONNECTION_H
#define ECHOCONNECT_ECHO_RAW_CONNECTION_H

#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>

/**
 * @brief Public API for raw data through sound sending provided by libecho.
 */
class EchoRawConnection {
public:
    /**
     * @brief Returns pointer to EchoRawConnection object which allows data audio transfer by encoding every bit of
     * information separately.
     *
     * For data transfer, frequencies in which we encode(send) and decode(receive) every bit
     * (respectively loFreq for 0 value and hiFreq for 1 value) are specified.
     * Then each bit is being send for a winSize long period of time.
     *
     * @warning Caller needs to remember about calling delete on return pointer or wrapping it in some smart_ptr!
     *
     * @param winSize       How much of bitrate we want to use to play sound in which we encode bit.
     * @param sendLoFreq    Frequency used to encode bits of value 0 when sending data.
     * @param sendHiFreq    Frequency used to encode bits of value 1 when sending data.
     * @param receiveLoFreq Frequency used to decode bits of value 0 when receving data.
     * @param receiveLoFreq Frequency used to decode bits of value 1 when receiving data.
     * @param leftLim       Silence magnitude
     * @param rightLim      Smaller of the 2 frequencies' magnitudes
     * @return              Pointer to desired EchoRawConnection object.
     */

    static EchoRawConnection *getBitEchoRawConnection(int win_size, int send_lo_freq, int send_hi_freq,
                                                      int recv_lo_freq, int recv_hi_freq, int left_lim, int right_lim);

    static EchoRawConnection *getBitEchoRawConnectionv2(int win_size, int send_freq, int recv_freq, int lim);

    virtual ~EchoRawConnection() = default;
    EchoRawConnection() = default;
    EchoRawConnection(const EchoRawConnection &other) = delete;
    EchoRawConnection(EchoRawConnection &&other) noexcept = default;
    EchoRawConnection &operator=(EchoRawConnection &other) = delete;
    EchoRawConnection &operator=(EchoRawConnection &&other) noexcept = default;

    /**
     * @brief Starts a message block
     */
    virtual void sendStart() = 0;

    /**
     * @brief Sends given byte sequence
     *
     * @param buffer    Bytes to send.
     * @param size      Size of buffer.
     */
    virtual void send(uint8_t *buffer, int size) = 0;

    /**
     * @brief Waits until all sendings finish.
     */
    virtual void sendWait() = 0;

    /**
     * @brief Waits for the start of the signal
     */
    virtual void receiveStart(std::chrono::duration<double> timeout) = 0;

    /**
     * @brief Tries to read @p size bytes, fills @p buffer with received bytes, returns number of read bytes.
     *
     * Before series of calling it receiveFirst must be called.
     * For now, there is no guarantee regarding correctness of transmission.
     * @param buffer    Where to write received bytes.
     * @param size      Size of data to read.
     * @return          Size of read data.
     */
    virtual int receive(uint8_t *buffer, int size) = 0;
};

#endif  // ECHOCONNECT_ECHO_RAW_CONNECTION_H
