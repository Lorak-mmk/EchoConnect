#ifndef ECHOCONNECT_AUDIOOUTPUT_H
#define ECHOCONNECT_AUDIOOUTPUT_H

#include <QtCore/QThread>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>

class AudioOutput : QObject {
    Q_OBJECT
public:
    AudioOutput();
    AudioOutput(const QAudioFormat& format);

    ~AudioOutput();

    QAudioOutput* getOutput();

    void start(QIODevice* data);
private:
    QAudioOutput output;
    QThread workerThread;

private slots:
    void handleStateChanged(QAudio::State newState);
    void handleNotify();
};

#endif  // ECHOCONNECT_AUDIOOUTPUT_H
