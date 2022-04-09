#include "client.h"
#include "keychain.h"
#include "message.h"

#include <stdexcept>
#include <cstring>

// #include <stdio.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define SPEAKER_TAG "[Client] "
#define NO_FLAGS 0


Client::Client(const std::string &) {
	this->Initialize();
}

Client::~Client(void) {
	this->Dispose();
}

void Client::Initialize(void) {
	this->InitMessageQueue();
	this->InitSharedMemory();
}

void Client::InitMessageQueue(void) {
	int key = KeyChain::GetKey();
	this->msgID = msgget(key, 0600);

	if (this->msgID == -1) {
		const char * msg = SPEAKER_TAG "msgget failed";
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		printf(SPEAKER_TAG "connected to message queue with ID=%d\n", this->msgID);
	}
}

void Client::InitSharedMemory(void) {
	#pragma message "InitSharedMemory unimplemented method"
}

void Client::RequestConnection(void) {
	unsigned long long D = rand() % 100;
	D = D < 80 ? 0 : 1;

	const Message msg[2] = {
		Message(GM_CONNECTION_REQ, "hi!"),
		Message(GM_CONNECTION_REQ, "STOP"),
	};
	printf(SPEAKER_TAG "send request(%ld) %s\n", msg[D].type, msg[D].text);
	int status = msgsnd(this->msgID, &msg[D], GM_MSG_SIZE, NO_FLAGS);
	if (status == -1) {
		perror(SPEAKER_TAG "msgsnd failed");
	} else {
		printf(SPEAKER_TAG "sent message(%ld): %s\n", msg[D].type, msg[D].text);
	}
	
}

void Client::Dispose(void) {
	this->DisposeSharedMemory();
	this->DisposeMessageQueue();
}

void Client::DisposeMessageQueue(void) {
	printf(SPEAKER_TAG "disconnected from message queue with ID=%d\n", this->msgID);
	this->msgID = -1;
}
void Client::DisposeSharedMemory(void) {
	#pragma message "DispSharedMemory unimplemented method"
}
