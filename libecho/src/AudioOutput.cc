#include "AudioOutput.h"
#include "AudioFormatFactory.h"

#include <QtCore/QEventLoop>
#include <QMetaMethod>
#include <QDebug>


AudioOutput::AudioOutput() : AudioOutput(AudioFormatFactory::getDefaultOutputFormat()){}

AudioOutput::AudioOutput(const QAudioFormat& format) : format(format) {
    qDebug() << "Constructing AudioOutput. Thread:" << thread();

    this->moveToThread(this);


    QThread::start();
    waitForTick();
}

AudioOutput::~AudioOutput() {
    qDebug() << "Destroying AudioOutput";
    this->quit();
    this->wait();
    delete qOutput;
}

void AudioOutput::enqueueData(const char* data, size_t length) {
    qDebug() << "Queueing" << length << "bytes to play";
    std::lock_guard<std::mutex> lock(writeMutex);
    buffer.append(data, length);
    tryWriteData();
}

void AudioOutput::startAudio() {
    static int index = metaObject()->indexOfMethod(QMetaObject::normalizedSignature("startAudio_slot()"));
    metaObject()->method(index).invoke(this);
    waitForTick();
}

void AudioOutput::stopAudio() {
    static int index = metaObject()->indexOfMethod(QMetaObject::normalizedSignature("stopAudio_slot()"));
    waitForState(QAudio::IdleState);
    metaObject()->method(index).invoke(this);
}

void AudioOutput::waitForTick() {
    std::unique_lock<std::mutex> lock(writeMutex);
    forTick.wait(lock);
}

void AudioOutput::waitForState(QAudio::State waitFor) {
    if(qOutput->state() == waitFor) {
        return;
    }
    std::unique_lock<std::mutex> lock(writeMutex);
    forState.wait(lock, [=]{return qOutput->state() == waitFor; });
}

void AudioOutput::tryWriteData() {
    size_t bytesToWrite = std::min(qOutput->bytesFree(), buffer.size());
    if(bytesToWrite == 0) {
        return;
    }

    qDebug() << "Pushing" << bytesToWrite << "bytes to output audio buffer";
    qDevice->write(buffer, bytesToWrite);
    buffer.remove(0, bytesToWrite);
}

void AudioOutput::handleStateChanged(QAudio::State newState){
    qDebug() << "State changed signal received. New state: " << newState;
    forState.notify_all();
}

void AudioOutput::handleNotify() {
    qDebug() << "Notify signal received";
    qDebug() << "Buffer free/size: " << qOutput->bytesFree() << " " << qOutput->bufferSize();
    {
        std::lock_guard<std::mutex> lock(writeMutex);
        tryWriteData();
    }
    forTick.notify_all();
}

void AudioOutput::run() {
    qDebug() << "Running thread " << thread();

    qOutput = new QAudioOutput(format, this);
    qOutput->setNotifyInterval(NOTIFY_INTERVAL);
    qOutput->setVolume(VOLUME);

    connect(&*qOutput, &QAudioOutput::stateChanged, this, &AudioOutput::handleStateChanged);
    connect(&*qOutput, &QAudioOutput::notify, this, &AudioOutput::handleNotify);

    forTick.notify_all();
    QThread::exec();
}

void AudioOutput::startAudio_slot() {
    qDebug() << "Start audio from thread" << thread();
    qDevice = qOutput->start();
    qDebug("\tBuffer size: %d\n\tPeriod size: %d\n\tNotify interval: %d\n\tVolume: %lf\n\t", qOutput->bufferSize(), qOutput->periodSize(), qOutput->notifyInterval(), qOutput->volume());
    forTick.notify_all();
}

void AudioOutput::stopAudio_slot() {
    qDebug() << "Stop audio from thread" << thread();
    qOutput->stop();
    qDevice = nullptr;
}
