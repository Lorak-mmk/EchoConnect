#ifndef ECHOCONNECT_AUDIOFORMATFACTORY_H
#define ECHOCONNECT_AUDIOFORMATFACTORY_H

#include <QtMultimedia/QAudioFormat>

class AudioFormatFactory {
public:
    static QAudioFormat getDefaultInputFormat();

    static QAudioFormat getDefaultOutputFormat();
};

struct AudioStreamInfo {
    int bufferSize;
    int notifyInterval;
    int periodSize;
    qreal volume;
};

#endif  // ECHOCONNECT_AUDIOFORMATFACTORY_H
