#ifndef ECHOCONNECT_AUDIOSTREAM_H
#define ECHOCONNECT_AUDIOSTREAM_H

#include <QtCore/QThread>
#include <QtMultimedia/QAudio>
#include <QtMultimedia/QAudioFormat>
#include <QDebug>

#include <mutex>
#include <condition_variable>

constexpr int NOTIFY_INTERVAL = 32;
constexpr double VOLUME = 1.0;

struct AudioStreamInfo {
    int bufferSize;
    int notifyInterval;
    int periodSize;
    double volume;
};


class AudioStreamSignalsAndSlots : public QThread {
    Q_OBJECT
protected slots:
    virtual void handleStateChanged_slot(QAudio::State newState) = 0;

    virtual void handleNotify_slot() = 0;
};

template<typename StreamType>
class AudioStream : public AudioStreamSignalsAndSlots {
public:
    using StatusType = std::pair<int, qint64>;

    explicit AudioStream(const QAudioFormat& format) : format(format) {
        qDebug() << "Constructing AudioStream. Thread:" << QThread::thread();
        this->moveToThread(this);
        QThread::start();
        waitForTick();
    }

    ~AudioStream() override {
        qDebug() << "Destroying AudioStream";
        this->quit();
        this->wait();
        delete qStream;
    }

    void waitForTick() {
        std::unique_lock<std::mutex> lock(mutex);
        forTick.wait(lock);
    }

    void waitForState(QAudio::State waitFor) {
        if(qStream->state() == waitFor) {
            return;
        }
        std::unique_lock<std::mutex> lock(mutex);
        forState.wait(lock, [=]{return qStream->state() == waitFor; });
    }

    AudioStreamInfo getStreamInfo(){
        return {
            .bufferSize = qStream->bufferSize(),
            .notifyInterval = qStream->notifyInterval(),
            .periodSize = qStream->periodSize(),
            .volume = qStream->volume()
        };
    }

    virtual StatusType getStreamStatus() = 0;

    virtual void handleNotify() = 0;

protected:
    StreamType *qStream{};
    QIODevice *qDevice{};

    std::mutex mutex{};

    void startStream() {
        qDebug() << "Starting stream from thread" << thread();
        qDevice = qStream->start();
        auto info = getStreamInfo();
        qDebug("\tBuffer size: %d\n\tPeriod size: %d\n\tNotify interval: %d\n\tVolume: %lf\n\t", info.bufferSize, info.periodSize, info.notifyInterval, info.volume);
        forTick.notify_all();
    }

    void stopStream() {
        qDebug() << "Stopping stream from thread" << thread();
        qStream->stop();
        qDevice = nullptr;
    }

private:
    QAudioFormat format{};

    std::condition_variable forTick{};
    std::condition_variable forState{};

    void run() override {
        qDebug() << "Running thread " << thread();

        qStream = new StreamType(format, this);
        qStream->setNotifyInterval(NOTIFY_INTERVAL);
        qStream->setVolume(VOLUME);

        connect(qStream, &StreamType::stateChanged, this, &AudioStream<StreamType>::handleStateChanged_slot);
        connect(qStream, &StreamType::notify, this, &AudioStream<StreamType>::handleNotify_slot);

        startStream();
        QThread::exec();
    }

    void handleNotify_slot() override {
        auto status = getStreamStatus();
        handleNotify();
        forTick.notify_all();
    }

    void handleStateChanged_slot(QAudio::State newState) override {
        qDebug() << "State changed signal received. New state: " << newState;
        forState.notify_all();
    };
};


#endif  // ECHOCONNECT_AUDIOSTREAM_H
