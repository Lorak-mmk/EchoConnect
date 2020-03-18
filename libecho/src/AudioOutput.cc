#include "AudioOutput.h"
#include "AudioFormatFactory.h"


AudioOutput::AudioOutput(QThread* eventThread) : AudioOutput(AudioFormatFactory::getDefaultOutputFormat(), eventThread){}

AudioOutput::AudioOutput(const QAudioFormat& output, QThread* eventThread) : output(output, this) {
    this->moveToThread(eventThread);
}

QIODevice *AudioOutput::start() {
    return output.start();
}
