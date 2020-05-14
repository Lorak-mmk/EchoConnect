#include "Calibrator.h"
#include <cmath>
#include <thread>

#define SAMPLE_RATE 44100
#define RISE 0.1
#define FALL 0.1

// TODO: move these into a seperate file

static double mag(double re, double im) {
    return sqrt(re * re + im * im);
}

static void dft_slide(const int16_t *buffer, int win_size, double ratio, double *out, int len) {
    double re = 0;
    double im = 0;
    double angle0 = 0;
    double angle1 = 0;
    double d_angle = 2.0 * M_PI * ratio;

    for (int i = 0; i < win_size; i++) {
        re += buffer[i] * std::cos(angle1);
        im += buffer[i] * std::sin(angle1);
        angle1 += d_angle;
    }
    out[0] = mag(re, im) / static_cast<double>(win_size);

    for (int i = 0; i < len - 1; i++) {
        re -= buffer[i] * std::cos(angle0);
        im -= buffer[i] * std::sin(angle0);
        re += buffer[i + win_size] * std::cos(angle1);
        im += buffer[i + win_size] * std::sin(angle1);
        out[i + 1] = mag(re, im) / static_cast<double>(win_size);
        angle0 += d_angle;
        angle1 += d_angle;
    }
}

Calibrator *Calibrator::getCalibrator(int win_size, int freq) {
    auto *cal = new Calibrator;
    auto fmt = getFormat();

    cal->win_size = win_size;
    cal->freq = freq;
    cal->input = std::make_unique<AudioInput>(fmt);
    cal->output = std::make_unique<AudioOutput>(fmt);

    return cal;
}

static void play(Calibrator *cal) {
    int i = 0;
    int t = 0;
    double a = 0;
    bool sig = false;
    int size = 4410;

    int16_t *buffer = new int16_t[size];

    cal->output->startStream();

    while (cal->playing) {
        buffer[i] = sin(6.283185307 * t * cal->freq / 44100) * 32766 * a;

        if (sig) {
            a += RISE;
            if (a >= 1.0)
                a = 1.0;
        } else {
            a -= FALL;
            if (a <= 0.0)
                a = 0.0;
        }

        i++;
        t++;
        if (t % cal->win_size == 0) {
            sig = !sig;
        }

        if (i >= size) {
            cal->output->enqueueData((char *)buffer, size);
            cal->output->waitForTick();
            i = 0;
        }
    }

    cal->output->stopStream();

    delete[] buffer;
}

void Calibrator::startPlayback() {
    playing = true;
    std::thread thr(play, this);
    thr.detach();
}

void Calibrator::stopPlayback() {
    playing = false;
}

double Calibrator::getLim(int skip_ms, int record_ms) {
    int skip_windows = ((int64_t)SAMPLE_RATE * skip_ms) / (win_size * 1000);
    int record_windows = ((int64_t)SAMPLE_RATE * record_ms) / (win_size * 1000);

    int16_t *samples = new int16_t[win_size * (record_windows + 1)];
    double *mags = new double[win_size * record_windows];

    input->startStream();

    for (int i = 0; i < skip_windows; i++) {
        int inc = 0;
        int bytes = win_size * 2;
        while (bytes > 0) {
            int nread = input->readBytes((char *)samples + inc, bytes);
            inc += nread;
            bytes -= nread;
        }
    }

    int inc = 0;
    int bytes = win_size * (record_windows + 1) * 2;
    while (bytes > 0) {
        int nread = input->readBytes((char *)samples + inc, bytes);
        inc += nread;
        bytes -= nread;
    }

    input->stopStream();

    dft_slide(samples, win_size, (double)freq / SAMPLE_RATE, mags, win_size * record_windows);
    std::sort(mags, mags + (win_size * record_windows));
    double lim = mags[win_size * record_windows / 2];

    delete[] samples;
    delete[] mags;

    return lim;
}

QAudioFormat Calibrator::getFormat() {
    QAudioFormat fmt;
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setChannelCount(1);
    fmt.setCodec("audio/pcm");
    fmt.setSampleRate(SAMPLE_RATE);
    fmt.setSampleSize(16);
    fmt.setSampleType(QAudioFormat::SignedInt);
    return fmt;
}
