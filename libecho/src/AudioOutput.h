#ifndef ECHOCONNECT_AUDIOOUTPUT_H
#define ECHOCONNECT_AUDIOOUTPUT_H

#include <QtCore/QThread>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>

class AudioOutput : QObject {
    Q_OBJECT
public:
    AudioOutput(QThread* eventThread);
    AudioOutput(const QAudioFormat& format, QThread* eventThread);

    QIODevice* start();

private:
    QAudioOutput output;
    QThread eventThread;
};

#endif  // ECHOCONNECT_AUDIOOUTPUT_H
