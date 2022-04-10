#include "messenger.h"
#include "keychain.h"

#include <stdexcept>
#include <cstring>

#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>

#define NO_FLAGS 0
using std::string;

Messenger::Messenger(const string & tag) : speaker_tag(tag) { }

const char * Messenger::Tag(const char * msg) const {
	static const int siz = 128;
	static char buf[siz];
	snprintf(buf, siz, "[%s] %s", speaker_tag.c_str(), msg);
	return buf;
}

// Sets msgID according to the message queue returned by the [shmget] syscall.
// - If create_new is true, will try to create a new queue.
void Messenger::InitMessageQueue(bool create_new) {
	int key = KeyChain::GetKey();
	int flags = 0600 | (create_new ? IPC_CREAT | IPC_EXCL : 0);
	this->msgID = msgget(key, flags);

	if (this->msgID == -1) {
		const char * msg = Tag("msget failed");
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		printf("[%s] message queue initialized with ID=%d\n", this->speaker_tag.c_str(), this->msgID);
	}
}


// Sets msgID according to the message queue returned by the [shmget] syscall.
// - If create_new_size > 0, will try to create a new shm block.
void Messenger::InitSharedMemory(int create_new_size) {
	int key = KeyChain::GetKey(true);
	int flags = 0600 | (create_new_size > 0 ? IPC_CREAT | IPC_EXCL : 0);
	this->shmID = shmget(key, create_new_size, flags);

	if (this->shmID == -1) {
		const char * msg = Tag("shmget failed");
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		printf("[%s] shared memory nitialized with ID=%d\n", this->speaker_tag.c_str(), this->msgID);
		this->RetrieveSharedMemory();
		this->AttachSharedMemory();
	}
}

void Messenger::RetrieveSharedMemory(void) {
	if (this->shmID == -1) {
		throw std::runtime_error(Tag("shared memory not initialized"));
	} else {
		shmid_ds info;
		int status = shmctl(this->shmID, IPC_STAT, &info);

		if (status == -1) {
			const char * error_msg = Tag("shmctl failed");
			perror(error_msg);
			throw std::runtime_error(error_msg);
		} else {
			this->NBYTES = info.shm_segsz;
			printf("[%s] shared memory size=%d\n", speaker_tag.c_str(), this->NBYTES);
		}
	}
}

void Messenger::AttachSharedMemory(void) {
	if (this->shmID == -1) {
		throw std::runtime_error(Tag("shared memory not initialized"));
	} else {
		this->shmAT = (char*) shmat(this->shmID, NULL, NO_FLAGS);

		if (this->shmAT != (char*) -1) {
			printf("[%s] attach OK\n", speaker_tag.c_str());
		} else {
			const char * error_msg = Tag("attach error");
			perror(error_msg);
			throw std::runtime_error(error_msg);
		}
	}
}

void Messenger::DetachSharedMemory(void) {
	if (this->shmAT != nullptr) {
		int status = shmdt(this->shmAT);
		if (status == -1) {
			perror(Tag("shmdt failed"));
		} else {
			printf("[%s] detach(%d) shared memory from address %p\n", speaker_tag.c_str(), status, this->shmAT);
			this->shmAT = nullptr;
		}
	}
}

void Messenger::DisposeSharedMemory(bool remove) {
	if (this->shmID != -1) {
		this->DetachSharedMemory();

		if (remove) {
			int status = shmctl(this->shmID, IPC_RMID, NULL);
			if (status == -1) {
				perror(Tag("shmctl failed"));
			} else {
				printf("[%s] released shared memory block ID=%d\n", speaker_tag.c_str(), this->shmID);
				this->shmID = -1;
			}
		} else {
			printf("[%s] disconnected from shared memory block ID=%d\n", speaker_tag.c_str(), this->shmID);
			this->shmID = -1;
			this->NBYTES = 0;
		}
	}
}

void Messenger::DisposeMessageQueue(bool remove) {
	if (this->msgID != -1) {
		if (remove) {
			int status = msgctl(this->msgID, IPC_RMID, NULL);
			if (status == -1) {
				perror(Tag("msgctl failed"));
			} else {
				printf("[%s] released message queue ID=%d\n", this->speaker_tag.c_str(), this->msgID);
				this->msgID = -1;
			}
		} else {
			printf("[%s] disconnected from message queue ID=%d\n", speaker_tag.c_str(), this->msgID);
			this->msgID = -1;
		}
	}
}

////////////////////////////

const Message Messenger::AwaitMessage(long msgtype) const {
	Message request;
	int status;
	
	printf("[%s] awaiting message\n", this->speaker_tag.c_str());
	status = msgrcv(this->msgID, &request, GM_MSG_SIZE, msgtype, NO_FLAGS);
	if (status == -1) {
		perror(Tag("msgrcv failed"));
	} else {
		printf("[%s] received message: %s\n", this->speaker_tag.c_str(), request.text);
	}

	return request;
}

void Messenger::SendMessage(long msgtype, const char * msgtext) const {
	Message request(msgtype, msgtext);

	printf("[%s] send message(%ld) %s\n", this->speaker_tag.c_str(), request.type, request.text);
	int status = msgsnd(this->msgID, &request, GM_MSG_SIZE, NO_FLAGS);
	if (status == -1) {
		perror(Tag("msgsnd failed"));
	} else {
		printf("[%s] sent message(%ld): %s\n", this->speaker_tag.c_str(), request.type, request.text);
	}
}

//void Messenger::ReadData(void) {
//	printf("[%s] SM area content:\n-----\n%s\n-----\n", this->speaker_tag.c_str(), this->shmAT);
//}

//void Messenger::WriteData(void) {
//	snprintf(this->shmAT, NBYTES, "SM area ready\nLast=%s", this->speaker_tag.c_str());
//}
