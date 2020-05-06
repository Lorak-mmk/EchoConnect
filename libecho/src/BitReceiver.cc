#include "BitReceiver.h"
#include <cmath>
#include "HammingCode.h"

static const int SAMPLE_RATE = 44100;
static const int SAMPLE_SIZE = 16;
static const int CHANNEL_COUNT = 1;
static const char *CODEC = "audio/pcm";
static const QAudioFormat::SampleType SAMPLE_TYPE = QAudioFormat::SignedInt;
static const QAudioFormat::Endian BYTEORDER = QAudioFormat::LittleEndian;

static double mag(double re, double im) {
    return sqrt(re * re + im * im);
}

static double dft(const int16_t *buffer, int win_size, double ratio) {
    double re = 0;
    double im = 0;
    double angle = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    double d_angle = 2.0 * M_PI * ratio;

    for (int i = 0; i < win_size; i++) {
        double val = buffer[i];
        re += val * std::cos(angle);
        im += val * std::sin(angle);
        angle += d_angle;
    }

    return mag(re, im) / static_cast<double>(win_size);
}

static void dftSlide(const int16_t *buffer, int win_size, double ratio, double *out, int len) {
    double re = 0;
    double im = 0;
    double angle0 = 0;
    double angle1 = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
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

QAudioFormat BitReceiver::getInputFormat() {
    QAudioFormat format;
    format.setByteOrder(BYTEORDER);
    format.setChannelCount(CHANNEL_COUNT);
    format.setCodec(CODEC);
    format.setSampleRate(SAMPLE_RATE);
    format.setSampleSize(SAMPLE_SIZE);
    format.setSampleType(SAMPLE_TYPE);
    return format;
}

BitReceiver::BitReceiver(int win_size, int lo_freq, int hi_freq, int left_lim, int right_lim)
    : IReceiver(getInputFormat(), win_size), left_lim(left_lim), right_lim(right_lim) {
    setLoFrequency(lo_freq);
    setHiFrequency(hi_freq);
    window.reserve(win_size);
    sync_in.reserve(3 * win_size);
    sync_lo_out.reserve(2 * win_size);
    sync_hi_out.reserve(2 * win_size);
}

long BitReceiver::getLoFrequency() {
    return std::lround(lo_ratio * SAMPLE_RATE);
}

long BitReceiver::getHiFrequency() {
    return std::lround(hi_ratio * SAMPLE_RATE);
}

void BitReceiver::setLoFrequency(int freq) {
    lo_ratio = static_cast<double>(freq) / SAMPLE_RATE;
}

void BitReceiver::setHiFrequency(int freq) {
    hi_ratio = static_cast<double>(freq) / SAMPLE_RATE;
}

void BitReceiver::readSamples(int16_t *buffer, int len) {
    int inc = 0;
    int bytes = len * 2;
    while (bytes > 0) {
        int nread = input->readBytes(reinterpret_cast<char *>(buffer) + inc, bytes);
        inc += nread;
        bytes -= nread;
        input->waitForTick();
    }
}

int BitReceiver::stepShift(const double *buffer, int size) {
    double sum = 0;
    double delta;
    double diff;
    // TODO: this cutoff point is loosely dependent on the window size and the
    // volume and noisiness of the microphone. I don't think there's a reasonable
    // way to compute this, but who knows. For now it's just hardcoded.
    double diff_min = 100;
    int res = -1;

    for (int i = 0; i < size; i++) {
        sum += buffer[i];
    }

    for (int i = 0; i < size; i++) {
        sum += buffer[i + size];
        delta = buffer[i + size] - buffer[i];
        if (delta < right_lim) {
            continue;
        }

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
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

int BitReceiver::decodeBit(int16_t *windowBuffer) {
    double lo = dft(windowBuffer, win_size, lo_ratio);
    double hi = dft(windowBuffer, win_size, hi_ratio);
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
        if (lo_shift != -1) {
            shift = lo_shift;
        }
        if (hi_shift != -1) {
            shift = hi_shift;
        }
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

void BitReceiver::receiveBits(std::vector<bool> &vec, int offset) {
    for (unsigned i = offset; i < vec.size(); i++) {
        readSamples(window.data(), win_size);
        vec[i] = decodeBit(window.data());
    }
}

/*
int BitReceiver::receiveFirst(uint8_t *buffer, int size) {
    HammingCode hamming;

    std::vector<bool> bits;
    bits.resize(HammingCode::encodedLength(size));

    int first_two = receiveFirstTwoBits();
    bits[0] = first_two & 1;
    bits[1] = first_two & 2;
    receiveBits(bits, 2);
    hamming.fixErrors(bits);
    std::vector<uint8_t> decoded = hamming.decode(bits);
    memcpy(buffer, decoded.data(), size);

    return size;
}
*/

void BitReceiver::start() {}

int BitReceiver::receive(uint8_t *buffer, int size) {
    HammingCode hamming;

    std::vector<bool> bits;
    bits.resize(HammingCode::encodedLength(size));

    receiveBits(bits, 0);
    hamming.fixErrors(bits);
    std::vector<uint8_t> decoded = hamming.decode(bits);
    memcpy(buffer, decoded.data(), size);

    return size;
}

void BitReceiver::skip() {
    double lo, hi;
    do {
        readSamples(window.data(), win_size);
        lo = dft(window.data(), win_size, lo_ratio);
        hi = dft(window.data(), win_size, hi_ratio);
    } while (lo > left_lim || hi > left_lim);
}
