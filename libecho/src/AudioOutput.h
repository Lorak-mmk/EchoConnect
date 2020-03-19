#ifndef ECHOCONNECT_AUDIOOUTPUT_H
#define ECHOCONNECT_AUDIOOUTPUT_H

#include <QtCore/QThread>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>

constexpr int NOTIFY_INTERVAL = 32;
constexpr double VOLUME = 1.0;

class AudioOutput : public QThread {
    Q_OBJECT
public:
    AudioOutput();

    explicit AudioOutput(const QAudioFormat &format);

    ~AudioOutput() override;

    void run() override;

    void enqueueData(const char *data, size_t length);

    void startAudio();

    void stopAudio();

    void waitForTick();

    void waitForState(QAudio::State waitFor);

private:
    void tryWriteData();

    QAudioOutput *qOutput{};
    QIODevice *qDevice{};
    QAudioFormat format{};

    QByteArray buffer{};

    std::mutex writeMutex{};
    std::condition_variable forTick{};
    std::condition_variable forState{};

private slots:
    void handleStateChanged(QAudio::State newState);

    void handleNotify();

    void startAudio_slot();

    void stopAudio_slot();
};

#endif  // ECHOCONNECT_AUDIOOUTPUT_H
