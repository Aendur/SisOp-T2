#include "client.h"

Client::Client(const char *) : messenger("Client") {
	this->seed = (long) std::random_device()();
	if (this->seed < 100) this->seed += 100;
	this->generator = std::mt19937_64(seed);

	messenger.InitMessageQueue(false);
	messenger.InitSharedMemory(0);
}

Client::~Client(void) {
	messenger.DisposeSharedMemory(false);
	messenger.DisposeMessageQueue(false);
}

void Client::Connect(void) {
	char buf[GM_MSG_SIZE];
	snprintf(buf, GM_MSG_SIZE, "%ld", this->seed);
	messenger.SendMessage(GM_CONNECTION_REQ, buf);
	
	Message m;
	if (messenger.AwaitMessage(this->seed, true, &m)) {
		m.text[4] = 0;
		m.text[9] = 0;
		m.text[14] = 0;
		m.text[19] = 0;
		int W = std::stoi(&m.text[0]);
		int H = std::stoi(&m.text[5]);
		int R = std::stoi(&m.text[10]);
		int G = std::stoi(&m.text[15]);
		int B = std::stoi(&m.text[20]);
		printf("%d,%d,%d,%d,%d\n", W, H, R, G, B);
		this->board.Initialize(W, H, messenger.GetAddress());
	}
}

// #include <thread>
// #include <chrono>
void Client::Run(void) {
	//std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(30000));
	getchar();
	messenger.SendMessage(GM_CONNECTION_END, "OK");
}
