#include <cmath>
#include "RawReceiver.h"

#define SAMPLE_RATE 44100

static double dft(const int16_t *buffer, int samples, double ratio) {
    double re = 0;
    double im = 0;
    double angle = 0;
    const double d_angle = 2.0 * M_PI * ratio;

    for (int i = 0; i < samples; i++) {
        double val = buffer[i];
        re += val * std::cos(angle);
        im += val * std::sin(angle);
        angle += d_angle;
    }

    return std::sqrt(re * re + im * im) / samples;
}

RawReceiver::RawReceiver(int loFreq, int hiFreq, double magLimitBegin, double magLimitEnd, int winSize) {
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setSampleRate(SAMPLE_RATE);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);

    input = std::make_unique<AudioInput>(format);

    this->loFreq = loFreq;
    this->hiFreq = hiFreq;
    this->magLimitBegin = magLimitBegin;
    this->magLimitEnd = magLimitEnd;
    this->winSize = winSize;
}



void RawReceiver::getBytes(char *buffer, int bytes) {
    int inc = 0;
    while (bytes > 0) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        int nread = input->readBytes(buffer + inc, bytes);
        inc += nread;
        bytes -= nread;
        input->waitForTick();
    }
}

void RawReceiver::clearInput() {
    int size = input->getStreamStatus().first;
    std::vector<char> dummy(size);
    input->readBytes(dummy.data(), size);
}

int RawReceiver::receive(char *buffer, int size) {
    int received_bits = 0;
    int received_bytes = 0;
    int samples = winSize / 2;  // we're dealing with half-windows
    double loRatio = (double)loFreq / SAMPLE_RATE;
    double hiRatio = (double)hiFreq / SAMPLE_RATE;
    double loMag;
    double hiMag;

    std::vector<int16_t> window(samples * 2);
    char *window_ptr = (char *) window.data();

    input->startStream();
    clearInput();

    qDebug() << "Listening for" << loFreq << "/" << hiFreq << "Hz";

    // wait for the first half-window and discard it
    do {
        getBytes(window_ptr, samples * 2);
        loMag = dft(window.data(), samples, loRatio);
        hiMag = dft(window.data(), samples, hiRatio);
           printf("%.9lf\n", std::max(loMag, hiMag));
    } while (loMag < magLimitEnd && hiMag < magLimitEnd);

    qDebug() << "Recording";

    // then, starting from the second half-window, read every other one
    while (true) {
        getBytes(window_ptr, samples * 2);
        loMag = dft(window.data(), samples, loRatio);
        hiMag = dft(window.data(), samples, hiRatio);
        qDebug() << std::max(loMag, hiMag);
        if (loMag < magLimitBegin && hiMag < magLimitBegin) {
            break;
        }
        getBytes(window_ptr, samples * 2);

        if (received_bits % 8 == 0)
            buffer[received_bytes] = 0;
        bool bit = loMag < hiMag;
        buffer[received_bytes] |= (bit << (received_bits & 7));
        received_bits++;
        received_bytes = received_bits >> 3;
        if (received_bytes == size)
            break;
    }

    input->stopStream();
    return received_bytes;
}
