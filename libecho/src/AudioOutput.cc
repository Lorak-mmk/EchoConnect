#include <QDebug>
#include "AudioOutput.h"
#include "AudioFormatFactory.h"


AudioOutput::AudioOutput() : AudioOutput(AudioFormatFactory::getDefaultOutputFormat()){}

AudioOutput::AudioOutput(const QAudioFormat& output) : output(output, this), workerThread() {
    this->moveToThread(&workerThread);
    connect(&this->output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
    connect(&this->output, SIGNAL(notify()), this, SLOT(handleNotify()));
    workerThread.start();
}

AudioOutput::~AudioOutput() {
    qWarning() << "Destroying AudioOutput";

    workerThread.quit();
    workerThread.wait();
}

QAudioOutput *AudioOutput::getOutput() {
    return &this->output;
}

void AudioOutput::handleStateChanged(QAudio::State newState){
    qWarning() << "New state: " << newState;
    qWarning() << "Buffer free/size: " << output.bytesFree() << " " << output.bufferSize();
    qWarning() << "periodSize(): " << output.periodSize();
}

void AudioOutput::handleNotify() {
    qWarning() << "Notify";
}
void AudioOutput::start(QIODevice * data) {

    output.start(data);
}
