#include "echo.h"
#include "AudioFormatFactory.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "QTInitializer.h"

#include <cmath>
#include <QCoreApplication>
#include <QDebug>

namespace {
    const int loRate = 400;
    const int hiRate = 500;
    const double windowTime = 0.1;
    using SampleType = int16_t;
    
    SampleType audio_wave(int frequency, int time, const QAudioFormat& format) {
        return sin(frequency * time / format.sampleRate()) * (1 << (format.sampleSize() - 1));
    }
    
    std::vector<SampleType> encode_byte(char byte, const QAudioFormat& format){
        std::vector<SampleType> encoded;
        for (int i = 0; i < 8; i++) {
            int freq = (byte >> i) & 1 ? hiRate : loRate;
            for (int t = 0; t < format.sampleRate() * windowTime; t++) {
                encoded.emplace_back(audio_wave(freq, t, format));
            }
        }
        return encoded;
    }
    
    std::vector<SampleType> encode_data(const std::vector<char>& data, const QAudioFormat& format) {
        std::vector<SampleType> encoded;
        for (const auto byte : data) {
            auto encoded_byte = encode_byte(byte, format);
            encoded.insert(encoded.end(), encoded_byte.begin(), encoded_byte.end());
        }
        return encoded;
    }
    
    void play(AudioOutput& audio, std::vector<SampleType>& buffer) {
        const size_t atOnce = 260; // (bitrate / notify_interval) + eps; - TODO: wyjaśnić
        
        audio.enqueueData(reinterpret_cast<char *> (buffer.data()), std::min(buffer.size(), atOnce));
        for(int i = atOnce; i <  buffer.size(); i += atOnce) {
            audio.enqueueData(reinterpret_cast<char *> (buffer.data() + i), std::min(atOnce, buffer.size() - i));
            audio.waitForTick();
        }
        
        audio.waitForState(QAudio::State::IdleState);
    }
}

void echo::send(const std::vector<char>& buffer) {
    static auto format = AudioFormatFactory::getDefaultOutputFormat();
    static AudioOutput audio(format);
    auto to_play = encode_data(buffer, format);
    play(audio, to_play);
}

std::vector<uint8_t> echo::receive() {
    static AudioInput audio(AudioFormatFactory::getDefaultInputFormat());
    auto info = audio.getStreamInfo();

    const size_t atOnce = info.periodSize;
    char *array = new char[atOnce];
    int idx = 0;
    const int time = 2000000;
    while (audio.getStreamStatus().second < time) {
        int read = audio.readBytes(array, atOnce - idx);
        idx = idx + read;
        if (idx == atOnce) {
            idx = 0;
            qDebug() << "Process data here - or better: schedule it so this loop is not blocked";
        }
        audio.waitForTick();
    }

    delete[] array;

    return std::vector<uint8_t>();
}

void echo::initEcho(int a_argc, char **a_argv) {
    static QTInitializer init{a_argc, a_argv};
}
