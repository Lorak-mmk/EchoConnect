#include "Chat.h"
#include "Config.h"
#include "Console.h"
#include "EchoProtocol.h"
#include "Utils.h"
#include "ConcurrentBuffer.h"
#include "InputField.h"

#include <unistd.h>
#include <sys/select.h>

constexpr char KEY_UP = 72;
constexpr char KEY_DOWN = 80;
constexpr char KEY_LEFT = 75;
constexpr char KEY_RIGHT = 77;
constexpr char NEW_LINE = '\n';

struct StateVariables {
	std::string user;
	EchoProtocol *protocol;
	InputField *input;
	ConcurrentBuffer *toSend, *chat;
	bool run;
}

void sendRecv(StateVariables *vars) {
	while (vars->run) {
		try {
			if (listen) {
				p.listen();
			} else {
				p.connect();
			}
		} catch (std::exception &e) {
			vars->run = false;
		}
	}
}

void drawChat(StateVariables *vars) {

}

void handleChar(StateVariables *vars, char c) {
	switch(c) {
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			vars->input->moveLeft();
			break;
		case KEY_RIGHT:
			vars->input->moveRight();
		case NEW_LINE:
			auto message = vars->input->getLastNLines(0, -1);
			vars->input->clear();
			vars->toSend->pushBack(setFormatting(ConsoleFormat::T_CYAN) + vars->user + ": "  + clearFormatting() + message);
			vars->chat->pushBack(setFormatting(ConsoleFormat::T_MAGENTA) + "You: " + clearFormatting() + message);
			break;
		default:
			vars->input->add(c);
			break;
	}
}

bool manageChatView(StateVariables *vars) {
    struct timeval timeout = {1, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

	while (vars->run) {
		int ret = select(1, &fds, NULL, NULL, &timeout);
		if (ret == -1) {
			return false;
		} else if (ret > 0) {
			char buffer[100];
			ssize_t n = read(STDIN_FILENO, &buffer, input);
			if (n < 0) {
				return false;
			}
			if (strncmp(buffer, "~end", n) == 0) {
				vars->run = false;
				return true;
			}
			for (size_t i = 0; i < n; i++) {
				handleChar(vars, buffer[i]);
			}
		}

		drawChat(vars);
	}

	return false;
}

ViewPtr Chat::runAction() {
    int winSize = std::get<int>(arguments.find("winSize")->second.value);
    int sendFreq = std::get<int>(arguments.find("sendFreq")->second.value);
    int recvFreq = std::get<int>(arguments.find("recvFreq")->second.value);
    int listen = std::get<int>(arguments.find("listen")->second.value);
    std::string username = std::get<std::string>(arguments.find("username")->second.value);

	InputField i;
	ConcurrentBuffer s, c;
    EchoProtocol p(winSize, sendFreq, recvFreq, (int)getMainConfig()->getLimFor(recvFreq, winSize, 0.0));
	StateVariables vars = { user = username, protocol = &p, input = &i, toSend = &s, chat = &c, run = true };

	std::cout << setFormatting({ConsoleFormat::T_BLUE}) << " We are analyzing if there is any ongoing transmission, please wait...\n" << clearFormatting();

	try {
		if (listen) {
			p.listen();
		} else {
			p.connect();
		}
	} catch (std::exception &e) {
		std::cout << setFormatting({ConsoleFormat::T_RED})
					<< " Unfortunately we could detect any transmission, press enter to return to the previous
					view...\n"
					<< clearFormatting();
		Utils::waitForEnter();
		return parent;
	}

	setNoCanon();
	setNoEcho();

	// tu robimy wątek
	drawChat(vars);

	p.close();

	setCanon();
	setEcho();

	if (!manageChatView(StateVariables *vars)) {
		std::cout << setCursor(4, 0) << clearLinesBelow();
		std::cout << setFormatting({ConsoleFormat::T_RED})
					<< " A system error occured, press enter to return to the previous view...\n"
					<< clearFormatting();
		Utils::waitForEnter();
	}

    return parent;
}