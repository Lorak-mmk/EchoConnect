#include "BitReceiver.h"
#include <cmath>

static const double SAMPLE_RATE = 44100.0;

static double mag(double re, double im) {
    return sqrt(re * re + im * im);
}

static double dft(int16_t *buffer, int win_size, double ratio) {
    double re = 0;
    double im = 0;
    double angle = 0;
    double d_angle = 2.0 * M_PI * ratio;

    for (int i = 0; i < win_size; i++) {
        double val = buffer[i];
        re += val * std::cos(angle);
        im += val * std::sin(angle);
        angle += d_angle;
    }

    return mag(re, im) / static_cast<double>(win_size);
}

static void dftSlide(int16_t *buffer, int win_size, double ratio, double *out, int len) {
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

// usused, here just in case we need it later
void BitReceiver::clearInput() {
    int size = input->getStreamStatus().first;
    std::vector<char> dummy(size);
    input->readBytes(dummy.data(), size);
}

BitReceiver::BitReceiver(int lo_freq, int hi_freq, int win_size, int mag_lim) {
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setSampleRate(SAMPLE_RATE);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);

    input = std::make_unique<AudioInput>(format);

    window.reserve(win_size);
    sync_in.reserve(3 * win_size);
    sync_lo_out.reserve(2 * win_size);
    sync_hi_out.reserve(2 * win_size);

    this->lo_ratio = static_cast<double>(lo_freq) / SAMPLE_RATE;
    this->hi_ratio = static_cast<double>(hi_freq) / SAMPLE_RATE;
    this->win_size = win_size;
    this->mag_lim = mag_lim;
}

void BitReceiver::readSamples(int16_t *buffer, int len) {
    int inc = 0;
    int bytes = len * 2;
    while (bytes > 0) {
        int nread = input->readBytes((char *)buffer + inc, bytes);
        inc += nread;
        bytes -= nread;
        input->waitForTick();
    }
}

int BitReceiver::stepShift(double *buffer, int size) {
    double sum = 0;
    double delta;
    double diff;
    // TODO: this cutoff point is loosely dependent on the window size and the
    // volume and noisiness of the microphone. I don't think there's a reasonable
    // way to compute this, but who knows. For now it's just hardcoded.
    double diff_min = 10;
    int res = -1;

    for (int i = 0; i < size; i++) {
        sum += buffer[i];
    }

    for (int i = 0; i < size; i++) {
        sum += buffer[i + size];
        delta = buffer[i + size] - buffer[i];
        if (delta < mag_lim)
            continue;

        diff = sum - 0.5 * ((size + 1) * (buffer[i + size] + buffer[i]));
        diff = std::abs(diff);
        if (diff_min > diff) {
            diff_min = diff;
            res = i;
        }
        sum -= buffer[i];
    }

    return res;
}

int BitReceiver::decodeBit(int16_t *window) {
    double lo = dft(window, win_size, lo_ratio);
    double hi = dft(window, win_size, hi_ratio);
    return static_cast<int>(lo < hi);
}

int BitReceiver::receiveFirstTwoBits() {
    int res = 0;
    int lo_shift = -1;
    int hi_shift = -1;
    int shift = -1;

    readSamples(sync_in.data(), win_size * 2);

    qDebug() << "Waiting for message start";

    do {
        readSamples(sync_in.data() + (2 * win_size), win_size);
        dftSlide(sync_in.data(), win_size, lo_ratio, sync_lo_out.data(), win_size * 2);
        dftSlide(sync_in.data(), win_size, hi_ratio, sync_hi_out.data(), win_size * 2);
        for (int i = 0; i < 2 * win_size; i++) {
            sync_in[i] = sync_in[i + win_size];
        }
        lo_shift = stepShift(sync_lo_out.data(), win_size);
        hi_shift = stepShift(sync_hi_out.data(), win_size);
        if (lo_shift != -1) shift = lo_shift;
        if (hi_shift != -1) shift = hi_shift;
    } while (shift == -1);

    qDebug() << "Message start detected!";

    for (int i = 0; i < 2 * win_size - shift; i++) {
        sync_in[i] = sync_in[i + win_size + shift];
    }
    readSamples(sync_in.data() + (2 * win_size - shift), shift);

    res += decodeBit(sync_in.data());
    res += decodeBit(sync_in.data() + win_size) << 1;

    return res;
}

int BitReceiver::receiveFirst(uint8_t *buffer, int size) {
    uint8_t byte = 0;

    input->startStream();
    clearInput();

    byte = receiveFirstTwoBits();
    for (int j = 2; j < CHAR_BIT; j++) {
        readSamples(window.data(), win_size);
        byte |= decodeBit(window.data()) << j;
    }
    buffer[0] = byte;

    for (int i = 1; i < size; i++) {
        byte = 0;
        for (int j = 0; j < CHAR_BIT; j++) {
            readSamples(window.data(), win_size);
            byte |= decodeBit(window.data()) << j;
        }
        buffer[i] = byte;
    }

    input->stopStream();

    return size;
}

int BitReceiver::receive(uint8_t *buffer, int size) {
    uint8_t byte;

    input->startStream();
    for (int i = 0; i < size; i++) {
        byte = 0;
        for (int j = 0; j < CHAR_BIT; j++) {
            readSamples(window.data(), win_size);
            byte |= decodeBit(window.data()) << j;
        }
        buffer[i] = byte;
    }
    input->stopStream();

    return size;
}
