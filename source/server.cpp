#include "server.h"
#include <cstring>

Server::Server(const std::string &) : messenger("Server") {
	messenger.InitMessageQueue(true);
	messenger.InitSharedMemory(2048);
}

Server::~Server(void) {
	messenger.DisposeSharedMemory(true);
	messenger.DisposeMessageQueue(true);
}

void Server::Run(void) {
	bool keepWaiting = true;
	messenger.WriteData();
	while (keepWaiting) {
		Message m = messenger.AwaitMessage(GM_CONNECTION_REQ);
		keepWaiting = (strcmp(m.text, "STOP") != 0);
		messenger.ReadData();
	}
}