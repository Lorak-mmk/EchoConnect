#include <QtMultimedia/QAudioFormat>
#include <memory>
#include "AudioInput.h"
#include "IReceiver.h"

class RawReceiver : public IReceiver {
public:
	RawReceiver(int loFreq, int hiFreq, double magLimitBegin, double magLimitEnd, int winSize);
	int receive(char *buffer, int size);
private:
	void getBytes(char *buffer, int bytes);
	void clearInput();

	QAudioFormat format;
	std::unique_ptr<AudioInput> input;

	int loFreq;
	int hiFreq;
	double magLimitBegin;
	double magLimitEnd;
	int winSize;
};
