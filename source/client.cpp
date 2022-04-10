#include "client.h"

#include <vector>

using std::vector, std::string;

Client::Client(const std::string &) : messenger("Client") {
	messenger.InitMessageQueue(false);
	messenger.InitSharedMemory(0);
}

Client::~Client(void) {
	messenger.DisposeSharedMemory(false);
	messenger.DisposeMessageQueue(false);
}

void Client::Run(void) {
	messenger.ReadData();
	messenger.WriteData();

	vector<string> msgs = {
		"hi!",
		"STOP"
	};

	int D = rand() % 100;
	int P = 80;
	messenger.SendMessage(GM_CONNECTION_REQ, msgs[D < P ? 0 : 1].c_str());
}