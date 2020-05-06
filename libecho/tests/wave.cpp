#include <math.h>
#include "AudioOutput.h"
#include "Echo.h"

#define BUFFER_SIZE 1000

int16_t buffer[BUFFER_SIZE];

int main(int argc, char **argv) {
    Echo::initEcho(argc, argv);

    if (argc != 2) {
        printf("usage: wave <freq>\n");
        return 1;
    }
    int freq = atoi(argv[1]);

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
    while (1) {
        buffer[i] = sin(6.283185307 * t * freq / 44100) * 32766;
        i++;
        t++;
        if (i == BUFFER_SIZE) {
            output.enqueueData((char *)buffer, 2 * BUFFER_SIZE);
            i = 0;
        }
    }
}
