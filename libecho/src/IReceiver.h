class IReceiver {
public:
    virtual int receive(uint8_t *buffer, int size) = 0;

private:
};
