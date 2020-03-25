#ifndef ECHOCONNECT_AUDIOSTREAM_H
#define ECHOCONNECT_AUDIOSTREAM_H

#include <QDebug>
#include <QtCore/QThread>
#include <QtMultimedia/QAudio>
#include <QtMultimedia/QAudioFormat>

#include <chrono>
#include <condition_variable>
#include <mutex>

/**
 * @brief Struct representing basic parameters of AudioStream.
 */
struct AudioStreamInfo {
    int bufferSize;     /**< Size of the internal input/output buffer. */
    int notifyInterval; /**< Interval between notify() signal emissions */
    int periodSize;     /**<  Slightly different for input and output stream. See docs of specific classes.
                           Output: https://doc.qt.io/qt-5/qaudiooutput.html#periodSize ,
                           Input: https://doc.qt.io/qt-5/qaudioinput.html#periodSize */
    double volume;      /**< Sound volume, between 0.0 and 1.0 */
};

/**
 * @brief Helper class for AudioStream<StreamType>.
 *
 * It is needed because Q_OBJECT macro doesn't support template classes.
 * One way to solve it is to move signals/slots to base, non-template class,
 * and override those methods in derived class (without using signals/slots keywords again).
 */
class AudioStreamSignalsAndSlots : public QThread {
    Q_OBJECT
protected slots:
    /**
     * @brief Slot called very time stream status changes.
     *
     * QAudio::State: https://doc.qt.io/qt-5/qaudio.html#State-enum
     * According to QT docs (https://doc.qt.io/qt-5/qaudiooutput.html), not all states are possible here:
     * "At any given time, the QAudioOutput will be in one of four states: active, suspended, stopped, or idle."
     * @param newState  Current state of qStream.
     */
    virtual void handleStateChanged_slot(QAudio::State newState) = 0;

    /**
     * @brief Slot called once in a configurable amount of time.
     *
     * It's based on amount of sound data processed, not real time.
     * See also: https://doc.qt.io/qt-5/qaudiooutput.html#setNotifyInterval
     */
    virtual void handleNotify_slot() = 0;
};

/**
 * @brief Class with common code for Audio Input/Output streams.
 *
 * @tparam StreamType   Underlying QT class. Shall be one of QAudioInput (for input stream), QAudioOutput (for output
 * stream).
 */
template <typename StreamType>
class AudioStream : public AudioStreamSignalsAndSlots {
public:
    /**
     * @brief Status of stream.
     *
     * First element is available bytes in buffer (free buffer for output, available bytes in buffer for input).
     * Output: https://doc.qt.io/qt-5/qaudiooutput.html#bytesFree
     * Input: https://doc.qt.io/qt-5/qaudioinput.html#bytesReady
     * Second element is microseconds of processed sound data.
     * Output: https://doc.qt.io/qt-5/qaudiooutput.html#processedUSecs
     * Input: https://doc.qt.io/qt-5/qaudioinput.html#processedUSecs
     */
    using StatusType = std::pair<int, qint64>;

    AudioStream() = delete;

    /**
     * @brief Constructors for AudioStream.
     *
     * Initializes members of class. Starts QThread which will process stream's signals.
     * Starts stream. Returns after thread and stream are started,
     * @param format    Format that will be used by stream.
     */
    explicit AudioStream(const QAudioFormat &format) : format(format) {
        qDebug() << "Constructing AudioStream. Thread:" << QThread::thread();
        this->moveToThread(this);
        QThread::start();
        waitForTick_NT();
    }

    /**
     * @brief Destroys AudioStream.
     *
     * Stops stream. Kills QThread and waits for it to end.
     * Currently produces console errors because of stopping timers from wrong thread.
     */
    ~AudioStream() override {
        qDebug() << "Destroying AudioStream";
        stopStream();
        this->quit();
        this->wait();
        delete qStream;
    }

    /**
     * @brief Returns next time notify() slot is called. Times out after notifyInterval() of stream.
     */
    void waitForTick() {
        std::unique_lock<std::mutex> lock(mutex);
        forTick.wait_for(lock, std::chrono::milliseconds(qStream->notifyInterval()));
    }

    /**
     * @brief Returns next time notify() slot is called - no-timeout version.
     */
    void waitForTick_NT() {
        std::unique_lock<std::mutex> lock(mutex);
        forTick.wait(lock);
    }

    /**
     * @brief Waits for qStream to be in given state.
     *
     * Returns immmediately if qStream is already in desired state.
     * Else returns after stateChanged(new_state) slot with desired state is called.
     * @param waitFor   State we are waiting for.
     */
    void waitForState(QAudio::State waitFor) {
        if (qStream->state() == waitFor) {
            return;
        }
        std::unique_lock<std::mutex> lock(mutex);
        forState.wait(lock, [=] { return qStream->state() == waitFor; });
    }

    /**
     * @brief Returns current stream information.
     *
     * Meaning of certain fields is explained in AudioStreamInfo docs.
     * @return  Struct with current informations about stream.
     */
    AudioStreamInfo getStreamInfo() {
        return {.bufferSize = qStream->bufferSize(),
                .notifyInterval = qStream->notifyInterval(),
                .periodSize = qStream->periodSize(),
                .volume = qStream->volume()};
    }

    /**
     * @brief Check current stream status. See docs for StatusType for info about returned data.
     *
     * @return Current stream status - available bytes in buffer and processed usecs.
     */
    virtual StatusType getStreamStatus() = 0;

    /**
     * @brief Custom actions to execute on notify() signal.
     */
    virtual void handleNotify() = 0;

protected:
    /**
     * @brief Underlying QT audio stream. One of: QAudioInput, QAudioOutput.
     */
    StreamType *qStream{};

    /**
     * @brief QIODevice representing audio device. Communication with audio device is done y reading/writing from/to
     * this device.
     */
    QIODevice *qDevice{};

    /**
     * @brief Mutex to avoid concurrent access to buffer in AudioOutput and to allow usage of conditional variables.
     */
    std::mutex mutex{};

    /**
     * @brief Starts the underlying QT audio stream.
     *
     * The implementation doesn't currently support stopping and restarting stream,because of problems with QMetaObject.
     * Doing so results in console errors.
     */
    void startStream() {
        qDebug() << "Starting stream from thread" << thread();
        qDevice = qStream->start();
        auto info = getStreamInfo();
        qDebug("\tBuffer size: %d\n\tPeriod size: %d\n\tNotify interval: %d\n\tVolume: %lf\n\t", info.bufferSize,
               info.periodSize, info.notifyInterval, info.volume);
        forTick.notify_all();
    }

    /**
     * @brief Stops the underlying QT audio stream.
     *
     * The implementation doesn't currently support stopping and restarting audio stream. Doing so results in console
     * errors.
     */
    void stopStream() {
        qDebug() << "Stopping stream from thread" << thread();
        qStream->stop();
        qDevice = nullptr;
    }

private:
    QAudioFormat format{};

    std::condition_variable forTick{};
    std::condition_variable forState{};

    /**
     * @brief Executed by QThread when starting it. Initializes and starts qStream.
     *
     * Currently the implementation doesn't support stopping and restarting the thread - it is alive for the whole
     * lifetime of object.
     */
    void run() override {
        qDebug() << "Running thread " << thread();

        qStream = new StreamType(format, this);

        connect(qStream, &StreamType::stateChanged, this, &AudioStream<StreamType>::handleStateChanged_slot);
        connect(qStream, &StreamType::notify, this, &AudioStream<StreamType>::handleNotify_slot);

        startStream();
        QThread::exec();
    }

    /**
     * @brief Implementation of handleNotify_slot from Current state of qStream.
     */
    void handleNotify_slot() override {
        handleNotify();
        forTick.notify_all();
    }

    /**
     * @brief Implementation of handleStateChanged slot from AudioStreamSignalsAndSlots
     *
     * @param newState  Current state of qStream.
     */
    void handleStateChanged_slot(QAudio::State newState) override {
        qDebug() << "State changed signal received. New state: " << newState;
        forState.notify_all();
    };
};

#endif  // ECHOCONNECT_AUDIOSTREAM_H
