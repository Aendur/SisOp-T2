#include "client.h"
#include "keychain.h"

Client::Client(const char *) {
	this->seed = (long) std::random_device()();
	if (this->seed < 100) this->seed += 100;
	this->generator = std::mt19937_64(seed);

	messenger.Retrieve(KeyChain::GetKey(KEY_MQ_CONNECTION));
	#pragma message "TODO attach shared memory"
}

Client::~Client(void) {
	#pragma message "TODO detach shared memory"
}

void Client::Connect(void) {
	char buf[GM_MSG_SIZE];
	snprintf(buf, GM_MSG_SIZE, "%ld", this->seed);
	messenger.Send(GM_CONNECTION_REQ, buf);
	
	Message m;
	if (messenger.Receive(this->seed, true, &m)) {
		#pragma message "TODO handle player ID"
		int id = std::stoi(m.text);
	}
}

// #include <thread>
// #include <chrono>
void Client::Run(void) {
	//std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(30000));
	getchar();
	messenger.Send(GM_CONNECTION_END, "OK");
}
