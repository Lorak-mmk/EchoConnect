class IReceiver {
public:
	virtual int receive(char *buffer, int size) = 0;
private:
};
