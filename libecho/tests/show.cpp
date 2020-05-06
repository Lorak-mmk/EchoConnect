#include <math.h>

#include "AudioInput.h"
#include "Echo.h"

#define BUFFER_SIZE 1000
#define RANGE 2000

int16_t buffer[BUFFER_SIZE];

int main(int argc, char **argv) {
    Echo::initEcho(argc, argv);

    QAudioFormat fmt;
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setChannelCount(1);
    fmt.setCodec("audio/pcm");
    fmt.setSampleRate(44100);
    fmt.setSampleSize(16);
    fmt.setSampleType(QAudioFormat::SignedInt);
    AudioInput input(fmt);
    input.startStream();

    while (1) {
        int nread = input.readBytes((char *)buffer, 2 * BUFFER_SIZE);
        if (nread & 1)
            return 1;
        nread >>= 1;
        for (int i = 0; i < nread; i++) {
            double w = ((int)buffer[i] + RANGE / 2) / (double)RANGE;
            if (w > 1.0)
                w = 1.0;
            for (int j = 0; j < w * 80; j++)
                putchar('#');
            putchar('\n');
        }
        input.waitForTick();
    }
}
