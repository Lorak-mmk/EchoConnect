#include <QDebug>
#include "AudioOutput.h"
#include "AudioFormatFactory.h"


AudioOutput::AudioOutput() : AudioOutput(AudioFormatFactory::getDefaultOutputFormat()){}

AudioOutput::AudioOutput(const QAudioFormat& output) : output(output, this) {
    connect(&this->output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
    connect(&this->output, SIGNAL(notify()), this, SLOT(handleNotify()));
}

AudioOutput::~AudioOutput() {
    qDebug() << "Destroying AudioOutput";
}

QAudioOutput *AudioOutput::getOutput() {
    return &this->output;
}

void AudioOutput::handleStateChanged(QAudio::State newState){
    qDebug() << "New state: " << newState;
    qDebug() << "Buffer free/size: " << output.bytesFree() << " " << output.bufferSize();
    qDebug() << "periodSize(): " << output.periodSize();
}

void AudioOutput::handleNotify() {
    qDebug() << "Notify";
}
void AudioOutput::start(QIODevice * data) {
    output.start(data);
}
