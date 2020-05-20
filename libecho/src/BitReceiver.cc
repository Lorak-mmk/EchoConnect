#include "BitReceiver.h"
#include <cmath>
#include "Dft.h"
#include "Exceptions.h"
#include "HammingCode.h"

static const int SAMPLE_RATE = 44100;
static const int SAMPLE_SIZE = 16;
static const int CHANNEL_COUNT = 1;
static const char *CODEC = "audio/pcm";
static const QAudioFormat::SampleType SAMPLE_TYPE = QAudioFormat::SignedInt;
static const QAudioFormat::Endian BYTEORDER = QAudioFormat::LittleEndian;

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

long BitReceiver::getLoFrequency() const {
    return std::lround(lo_ratio * SAMPLE_RATE);
}

long BitReceiver::getHiFrequency() const {
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

int BitReceiver::stepShift(const double *buffer, int size) const {
    double diff_min = 20 * win_size;  // measured empirically
    int res = -1;

    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += buffer[i];
    }

    for (int i = 0; i < size; i++) {
        sum += buffer[i + size];
        double delta = buffer[i + size] - buffer[i];
        if (delta < right_lim) {
            continue;
        }

        double diff = sum - ((size + 1) * (buffer[i + size] + buffer[i])) / 2;
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

void BitReceiver::start(std::chrono::duration<double> timeout) {
    int lo_shift = -1;
    int hi_shift = -1;
    int shift = -1;
    auto start = std::chrono::system_clock::now();

    readSamples(sync_in.data(), win_size * 2);

    qDebug() << "Waiting for message start";

    do {
        readSamples(sync_in.data() + (2 * win_size), win_size);
        dft_slide(sync_in.data(), win_size, lo_ratio, sync_lo_out.data(), win_size * 2);
        dft_slide(sync_in.data(), win_size, hi_ratio, sync_hi_out.data(), win_size * 2);
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
        if (std::chrono::system_clock::now() - start > timeout) {
            throw ConnectionBroken{};
        }
    } while (shift == -1);

    qDebug() << "Message start detected!";

    for (int i = 0; i < 2 * win_size - shift; i++) {
        sync_in[i] = sync_in[i + win_size + shift];
    }
    readSamples(sync_in.data() + (2 * win_size - shift), shift);

    first_two_bits = 0;
    first_two_bits += decodeBit(sync_in.data());
    first_two_bits += decodeBit(sync_in.data() + win_size) << 1;
}

void BitReceiver::receiveBits(std::vector<bool> &vec, int offset) {
    for (unsigned i = offset; i < vec.size(); i++) {
        readSamples(window.data(), win_size);
        vec[i] = decodeBit(window.data());
    }
}

int BitReceiver::receive(uint8_t *buffer, int size) {
    HammingCode hamming;

    std::vector<bool> bits;
    bits.resize(HammingCode::encodedLength(size));

    if (first_two_bits == -1) {
        receiveBits(bits, 0);
    } else {
        bits[0] = first_two_bits & 1;
        bits[1] = first_two_bits & 2;
        receiveBits(bits, 2);
        first_two_bits = -1;
    }
    hamming.fixErrors(bits);
    std::vector<uint8_t> decoded = hamming.decode(bits);
    memcpy(buffer, decoded.data(), size);

    return size;
}
