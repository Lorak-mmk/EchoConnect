#ifndef ECHOCONNECT_AUDIOOUTPUT_H
#define ECHOCONNECT_AUDIOOUTPUT_H

#include <QtCore/QThread>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>

class AudioOutput : public QThread {
    Q_OBJECT
public:
    AudioOutput();

    explicit AudioOutput(const QAudioFormat& format);

    ~AudioOutput() override;

    void start() ;

    void run() override;

    void enqueueData(const char *data, size_t length);

    void waitForTick();

private:
    void tryWriteData();

    QAudioOutput *qOutput{};
    QIODevice *qDevice{};
    QAudioFormat format;

    QByteArray buffer;

    std::mutex writeMutex;
    std::condition_variable forTick;

private slots:
    void handleStateChanged(QAudio::State newState);

    void handleNotify();

public slots:
    void startAudio();
};

#endif  // ECHOCONNECT_AUDIOOUTPUT_H
