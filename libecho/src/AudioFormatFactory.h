#ifndef ECHOCONNECT_AUDIOFORMATFACTORY_H
#define ECHOCONNECT_AUDIOFORMATFACTORY_H

#include <QtMultimedia/QAudioFormat>

/**
 * TODO
 */
class AudioFormatFactory {
public:
    /**
     * @brief   Get stream input format to use within library.
     *
     * @return  Returns QAudioFormat with default parameters used by library. Parameters are set in .cc file.
     */
    static QAudioFormat getDefaultInputFormat();

    /**
     * @brief   Get stream ouput format to use within library.
     *
     * @return  Returns QAudioFormat with default parameters used by library. Parameters are set in .cc file.
     */
    static QAudioFormat getDefaultOutputFormat();
};

#endif  // ECHOCONNECT_AUDIOFORMATFACTORY_H
