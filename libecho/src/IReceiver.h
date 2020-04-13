#ifndef IRECEIVER_H
#define IRECEIVER_H

class IReceiver {
public:
    virtual int receive(uint8_t *buffer, int size) = 0;

private:
};

#endif
