#include <math.h>
#include "AudioOutput.h"
#include "Echo.h"

#define RISE 0.1
#define FALL 0.1

int main(int argc, char **argv) {
    Echo::initEcho(argc, argv);

    if (argc < 3) {
        printf("usage: alt <win_size> <freq> [mask]\n");
        return 1;
    }
    int win_size = atoi(argv[1]);
    int freq = atoi(argv[2]);
    unsigned char mask = 0x55;
    if (argc > 3)
        mask = strtoul(argv[3], NULL, 0);

    int16_t *dwindow = new int16_t[win_size];

    QAudioFormat fmt;
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setChannelCount(1);
    fmt.setCodec("audio/pcm");
    fmt.setSampleRate(44100);
    fmt.setSampleSize(16);
    fmt.setSampleType(QAudioFormat::SignedInt);
    AudioOutput output(fmt);
    output.startStream();

    int i = 0;
    int t = 0;
    double a = 0;
    while (1) {
        dwindow[i] = sin(6.283185307 * t * freq / 44100) * 32766 * a;

        if (mask & 1) {
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
        if (i == win_size) {
            output.enqueueData((char *)dwindow, 2 * win_size);
            i = 0;
            mask = (mask >> 1) | (mask << 7);
        }
    }
}
