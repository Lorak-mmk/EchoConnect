#include <QtCore/QEventLoop>
#include <QDebug>
#include "AudioOutput.h"
#include "AudioFormatFactory.h"

AudioOutput::AudioOutput() : AudioOutput(AudioFormatFactory::getDefaultOutputFormat()){}

AudioOutput::AudioOutput(const QAudioFormat& format) : QThread(),  format(format){
    this->moveToThread(this);
    this->startAudio();
}

AudioOutput::~AudioOutput() {
    qDebug() << "Destroying AudioOutput";
    this->quit();
    this->wait();
}

void AudioOutput::enqueueData(const char* data, size_t length) {
    qDebug() << "Queueing " << length << " bytes to play";
    std::lock_guard<std::mutex> lock(writeMutex);
    buffer.append(data, length);
    tryWriteData();
}

void AudioOutput::waitForTick() {
    std::unique_lock<std::mutex> lock(writeMutex);
    forTick.wait(lock);
}

void AudioOutput::tryWriteData() {
    size_t bytesToWrite = std::min(qOutput->bytesFree(), buffer.size());
    if(bytesToWrite == 0) return;

    qDevice->write(buffer, bytesToWrite);
    buffer.remove(0, bytesToWrite);
}

void AudioOutput::handleStateChanged(QAudio::State newState){
    qDebug() << "New state: " << newState;
}

void AudioOutput::handleNotify() {
    qDebug() << "Notify";
    qDebug() << "Buffer free/size: " << qOutput->bytesFree() << " " << qOutput->bufferSize();
    {
        std::lock_guard<std::mutex> lock(writeMutex);
        tryWriteData();
    }
    forTick.notify_all();
}

void AudioOutput::run() {
    qDebug() << "Running thread";

    this->qOutput = new QAudioOutput(format);
    qOutput->setNotifyInterval(128);
    qOutput->moveToThread(this);


    connect(qOutput, &QAudioOutput::stateChanged, this, &AudioOutput::handleStateChanged);
    connect(qOutput, &QAudioOutput::notify, this, &AudioOutput::handleNotify);

    qDevice = qOutput->start();
    forTick.notify_all();
    QThread::exec();
}
void AudioOutput::startAudio() {
    QThread::start();
    waitForTick();
}
