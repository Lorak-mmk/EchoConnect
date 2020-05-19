#include "Calibrator.h"
#include <cmath>
#include <thread>
#include "Math.h"

static const int SAMPLE_RATE = 44100;
static const double RISE = 0.1;
static const double FALL = 0.1;

Calibrator *Calibrator::getCalibrator(int win_size, int freq) {
    auto *cal = new Calibrator(win_size, freq);
    auto fmt = getFormat();
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

    auto *buffer = new int16_t[size];

    cal->output->startStream();

    while (cal->playing) {
        buffer[i] = sin(2.0 * PI * t * cal->freq / SAMPLE_RATE) * ((1 << 15) - 2) * a;

        if (sig) {
            a += RISE;
            if (a >= 1.0) {
                a = 1.0;
            }
        } else {
            a -= FALL;
            if (a <= 0.0) {
                a = 0.0;
            }
        }

        i++;
        t++;
        if (t % cal->win_size == 0) {
            sig = !sig;
        }

        if (i >= size) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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
    int skip_windows = (SAMPLE_RATE * skip_ms) / (win_size * 1000);
    int record_windows = (SAMPLE_RATE * record_ms) / (win_size * 1000);

    auto *samples = new int16_t[win_size * (record_windows + 1)];
    auto *mags = new double[win_size * record_windows];

    input->startStream();

    for (int i = 0; i < skip_windows; i++) {
        int inc = 0;
        int bytes = win_size * 2;
        while (bytes > 0) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
            int nread = input->readBytes((char *)samples + inc, bytes);
            inc += nread;
            bytes -= nread;
        }
    }

    int inc = 0;
    int bytes = win_size * (record_windows + 1) * 2;
    while (bytes > 0) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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
