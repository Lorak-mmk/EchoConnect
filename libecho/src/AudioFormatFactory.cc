#include "AudioFormatFactory.h"

const QAudioFormat::Endian INPUT_BYTEORDER = QAudioFormat::LittleEndian;
const int INPUT_CHANNEL_COUNT = 1;
const char *INPUT_CODEC = "audio/pcm";
const int INPUT_SAMPLERATE = 44100;
const int INPUT_SAMPLESIZE = 16;
const QAudioFormat::SampleType INPUT_SAMPLETYPE = QAudioFormat::SignedInt;

QAudioFormat AudioFormatFactory::getDefaultInputFormat() {
    QAudioFormat result;

    result.setByteOrder(INPUT_BYTEORDER);
    result.setChannelCount(INPUT_CHANNEL_COUNT);
    result.setCodec(INPUT_CODEC);
    result.setSampleRate(INPUT_SAMPLERATE);
    result.setSampleSize(INPUT_SAMPLESIZE);
    result.setSampleType(INPUT_SAMPLETYPE);

    return result;
}

const QAudioFormat::Endian OUTPUT_BYTEORDER = QAudioFormat::LittleEndian;
const int OUTPUT_CHANNEL_COUNT = 1;
const char *OUTPUT_CODEC = "audio/pcm";
const int OUTPUT_SAMPLERATE = 44100;
const int OUTPUT_SAMPLESIZE = 8;
const QAudioFormat::SampleType OUTPUT_SAMPLETYPE = QAudioFormat::UnSignedInt;

QAudioFormat AudioFormatFactory::getDefaultOutputFormat() {
    QAudioFormat result;

    result.setByteOrder(OUTPUT_BYTEORDER);
    result.setChannelCount(OUTPUT_CHANNEL_COUNT);
    result.setCodec(OUTPUT_CODEC);
    result.setSampleRate(OUTPUT_SAMPLERATE);
    result.setSampleSize(OUTPUT_SAMPLESIZE);
    result.setSampleType(OUTPUT_SAMPLETYPE);

    return result;
}