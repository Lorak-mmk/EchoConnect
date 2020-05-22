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
using ssize_t = long;
#endif

class EchoProtocol {
public:
    /**
     * Constructs an EchoProtocol object, which can be used for sending
     * and receiving data, providing their correctness.
     * There should exist at most one EchoProtocol object at a time.
     */
    EchoProtocol(int winsize, int send_freq, int recv_freq, int lim);

    /**
     * Destructs an EchoProtocol object and closes an underlying
     * audio stream if needed
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
     */
    void close();

    /**
     * @brief           Reads at most @p count bytes from audio input and fills buffer with them.
     * @param buf       pointer to buffer that should be filled
     * @param count     number of bytes to read
     * @param timeout   number of seconds; if no signal is detected by then, function assumes
     *                  that connection is broken;
     *                  timeout < 0 means that function can wait infinitely
     *
     * @return          Returns number of read bytes (can be any from 0 to count),
     *                  or -1 in case of failure or end of stream.
     */
    ssize_t read(void *buf, size_t count, int timeout);

    /**
     * @brief         Writes @p count bytes to audio output.
     *                Can throw an exception when sent data isn't being confirmed.
     *
     * @param buf     pointer to buffer with data
     * @param count   number of bytes to send
     *
     * @return        Returns number of sent bytes (should be equal to count)
     */
    size_t write(const void *buf, size_t count);

private:
    std::atomic<bool> closed = false, is_connected = false;
    uint16_t number = 0;

    std::mutex m_send, m_recv;
    std::condition_variable cv_send, cv_recv;
    std::thread *thr = nullptr;

    std::unique_ptr<EchoRawConnection> connection;
    std::chrono::duration<double> big_win_size;

    std::vector<uint8_t> buffer_send;
    std::deque<uint8_t> buffer_recv;
    uint8_t *buffer;

    /**
     * @brief Thread sending data
     *
     * @param first       boolean value that indicates if this thread
     *                    is being started first (before receiving thread)
     */
    void thread(bool connecting);
};

#endif  // ECHOCONNECT_ECHO_PROTOCOL_H
