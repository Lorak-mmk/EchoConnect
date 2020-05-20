#include "SendText.h"
#include "EchoProtocol.h"
#include "Config.h"
#include "Utils.h"
#include "Console.h"
#include "Utils.h"

ViewPtr SendText::runAction() {
	int winSize = std::get<int>(arguments.find("winSize")->second.value);
	int sendFreq = std::get<int>(arguments.find("sendFreq")->second.value);
	int recvFreq = std::get<int>(arguments.find("recvFreq")->second.value);

	EchoProtocol protocol(winSize, sendFreq, recvFreq, getMainConfig()->getLimFor(recvFreq, winSize));

	std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " Please enter text you want to send: ";

	try {
		protocol.connect();
	} catch (std::exception &e) {
		std::cout << setFormatting({ConsoleFormat::T_RED}) << " Unfortunately we couldn't connect with any host willing to receive message, press enter to return to the previous view...\n" << clearFormatting();
		Utils::waitForEnter();
	}

//     try {
//         p.connect();
//         p.write("abcdefghijkl", 12);
//         p.write("mmmmmmmmmmmm", 12);
//         p.close();
//     } catch (std::exception &e) {
//         puts(e.what());
//     }

    std::cout << setFormatting({ConsoleFormat::T_YELLOW}) << "\n\n  Message sent successfuly, press enter to return to the previous view..." << clearFormatting();
    Utils::waitForEnter();

    return parent;
}