#ifndef ECHOCONNECT_ECHO_PROTOCOL_H
#define ECHOCONNECT_ECHO_PROTOCOL_H

#include "EchoRawConnection.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>

#ifdef _WIN32
#ifndef SSIZE_DEFINED
#define SSIZE_DEFINED
using ssize_t = long long;
#endif
#endif

/** Class that wraps protocol allowing to send data through audio,
 *  providing a stream-oriented transmission with reliability.
 */
class EchoProtocol {
public:
    /**
     * Constructs an EchoProtocol object, which can be used for sending
     * and receiving data, providing their correctness.
     * There should exist at most one EchoProtocol object at a time
     * (because if two objects existed, then sounds from one of them
     * would be considered noises for another and microphones aren't
     * usually reliable enough to make it work).
     *
     * @param winsize     window size
     * @param send_freq   frequency of sending
     * @param recv_freq   frequency of receiving
     * @param lim         volume limit, from which underlying raw connection
     *                    assumes that some data is being sent
     */
    EchoProtocol(int winsize, int send_freq, int recv_freq, int lim);

    /**
     * Destructs an EchoProtocol object and closes an underlying
     * audio stream if needed.
     */
    ~EchoProtocol();

    EchoProtocol(const EchoProtocol &other) = delete;
    EchoProtocol(EchoProtocol &&other) = delete;
    EchoProtocol &operator=(const EchoProtocol &other) = delete;
    EchoProtocol &operator=(EchoProtocol &&other) = delete;

    /**
     * @brief Waits for signal start from the other computer.
     */
    void listen();

    /**
     * @brief Connects with another computer, which should be waiting for signal.
     */
    void connect();

    /**
     * @brief Closes a connection.
     * Connection that was once opened should be closed (even when one of read/write
     * functions throws an exception).
     * Doesn't throw exceptions.
     *
     * Closing an already closed connection, or calling close() by a different thread
     * that called listen() or connect() is undefined behaviour.
     */
    void close();

    /**
     * @brief           Reads at most @p count bytes from audio input and fills buffer with them.
     * @param buf       pointer to buffer that should be filled
     * @param count     number of bytes to read
     * @param timeout   number of seconds; if no signal is detected by then, function assumes
     *                  that connection is broken; some signals are being continuously sent
     *                  by protocol to keep connection alive even when no data is being sent;
     *                  timeout < 0 means that function can wait infinitely
     *
     * @return          Returns number of read bytes (can be any from 0 to count),
     *                  or -1 in case of failure or end of stream.
     *                  After returning -1, program should call EchoProtocol::close().
     *                  Getting lower byte count than expected (especially 0) is not
     *                  an error, you should just try again.
     */
    ssize_t read(void *buf, size_t count, int timeout);

    /**
     * @brief         Writes @p count bytes to audio output.
     *                Can throw an exception (ConnectionBroken)
     *                when sent data isn't being confirmed
     *                (this means some problems in lower layer).
     *
     * @param buf     pointer to buffer with data
     * @param count   number of bytes to send
     *
     * @return        Returns number of sent bytes (is equal to count when any exception
     *                isn't being thrown).
     */
    size_t write(const void *buf, size_t count);

private:
    /// @brief Flag that indicates whether you can still write data to EchoProtocol object.
    std::atomic<bool> closed = false;

    /// @brief Flag that indicates whether connection exists or not.
    std::atomic<bool> is_connected = false;

    /// @brief Packet counter.
    uint16_t number = 0;

    /// @brief Mutexes and conditional variables for synchronization.
    std::mutex m_send, m_recv;
    std::condition_variable cv_send, cv_recv;

    /// @brief Thread sending and receiving data.
    std::thread *thr = nullptr;

    /// @brief Raw connection object used to provide data transfer on lower layer.
    std::unique_ptr<EchoRawConnection> connection;

    /// @brief Time interval that should be bigger then packet transfer time.
    std::chrono::duration<double> big_win_size;

    /// @brief Buffers for data queued for sending and for data already received.
    std::vector<uint8_t> buffer_send;
    std::deque<uint8_t> buffer_recv;
    uint8_t *buffer;

    /**
     * @brief Thread sending data
     *
     * @param first       Boolean value that indicates if this thread
     *                    is being started first (before receiving thread).
     */
    void thread(bool connecting);
};

#endif  // ECHOCONNECT_ECHO_PROTOCOL_H
