#ifndef ECHOCONNECT_AUDIOFORMATFACTORY_H
#define ECHOCONNECT_AUDIOFORMATFACTORY_H

#include <QtMultimedia/QAudioFormat>

class AudioFormatFactory {
public:
    static QAudioFormat getDefaultInputFormat();

    static QAudioFormat getDefaultOutputFormat();
};

#endif  // ECHOCONNECT_AUDIOFORMATFACTORY_H
