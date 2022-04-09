#include "server.h"
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

#define SPEAKER_TAG "[Server] "
#define NO_FLAGS 0


Server::Server(const std::string &) {
	this->Initialize();
}

Server::~Server(void) {
	this->Dispose();
}

void Server::Initialize(void) {
	this->InitMessageQueue();
	this->InitSharedMemory();
}

void Server::InitMessageQueue(void) {
	int key = KeyChain::GetKey();
	this->msgID = msgget(key, IPC_CREAT | IPC_EXCL | 0600);

	if (this->msgID == -1) {
		const char * msg = SPEAKER_TAG "msgget failed";
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		printf(SPEAKER_TAG "message queue initialized with ID=%d\n", this->msgID);
	}
}

void Server::InitSharedMemory(void) {
	#pragma message "InitSharedMemory unimplemented method"

	/*int flag = IPC_CREAT | IPC_EXCL | 0600;
	int key = KeyChain::GetKey(true);
	int smid = shmget(key, 1024, flag);
	
	// std::ofstream stream("smid.dat");
	// stream << smid << std::endl;
	// stream.close();

	const char * msg = "SM ready";
	char * board = (char*) shmat(smid, NULL, 0);
	if (board != (char*) -1) {
		std::cout << "server: att OK" << std::endl;
		strncpy (board, msg, 8);
		getchar();
		int status = shmdt(board);
		std::cout << "detached status " << status << std::endl;

	} else {
		std::cout << "server: att error" << std::endl;
		const char * error = "server error";
		perror(error);
	}

	int ctlcmd = IPC_RMID;
	int status = shmctl(smid, ctlcmd, NULL);
	std::cout << "ctl status " << status << std::endl;*/
}

void Server::Await(void) {
	Message request;
	int status;
	
	bool keepWaiting = true;
	while (keepWaiting) {
		printf(SPEAKER_TAG "awaiting for client\n");
		status = msgrcv(this->msgID, &request, GM_MSG_SIZE, GM_CONNECTION_REQ, NO_FLAGS);
		if (status == -1) {
			perror(SPEAKER_TAG "msgrcv failed");
		} else {
			printf(SPEAKER_TAG "msgrcv: %s\n", request.text);
			if (strcmp(request.text, GM_CMD_STOP) == 0) {
				keepWaiting = false;
			}
		}
	}
}

void Server::Dispose(void) {
	this->DisposeSharedMemory();
	this->DisposeMessageQueue();
}

void Server::DisposeMessageQueue(void) {
	//free msg queue
	int status = msgctl(this->msgID, IPC_RMID, NULL);
	if (status == -1) {
		perror("parent");
	} else {
		printf(SPEAKER_TAG "released message queue with ID=%d\n", this->msgID);
		this->msgID = -1;
	}
}
void Server::DisposeSharedMemory(void) {
	#pragma message "DispSharedMemory unimplemented method"
}
