#include "message_queue.h"

#include <stdexcept>
#include <cstring>
#include <sys/msg.h>
#include <errno.h>


// Sets msgID according to the message queue returned by the [shmget] syscall.
// - If create_new is true, will try to create a new queue.
void MessageQueue::Create(int key) {
	this->_id = msgget(key, IPC_CREAT | IPC_EXCL | 0600);

	if (this->_id == -1) {
		const char * msg = "msget failed";
		perror(msg);
		_key = -1;
		throw std::runtime_error(msg);
	} else {
		_key = key;
		printf("message queue initialized Key=%d ID=%d\n", this->_key, this->_id);
	}
}

void MessageQueue::Retrieve(int key) {
	this->_id = msgget(key, 0600);

	if (this->_id == -1) {
		const char * msg = "msget failed";
		perror(msg);
		_key = -1;
		throw std::runtime_error(msg);
	} else {
		_key = key;
		printf("message queue retrieved Key=%d ID=%d\n", this->_key, this->_id);
	}
}

void MessageQueue::Dispose(void) {
	if (this->_id != -1) {
		int status = msgctl(this->_id, IPC_RMID, NULL);
		if (status == -1) {
			perror("msgctl failed");
		} else {
			printf("message queue released Key=%d ID=%d\n", this->_key, this->_id);
			this->_id = -1;
			this->_key = -1;
		}
	}
}

////////////////////////////
#define NO_FLAGS 0

bool MessageQueue::Receive(long msgtype, bool await, Message * msgout) const {
	int status;
	int flags;
	if (await) {
		flags = NO_FLAGS;
		printf("awaiting message\n");
	} else {
		flags = IPC_NOWAIT;
		printf("polling message\n");
	}
	
	status = msgrcv(this->_id, msgout, GM_MSG_SIZE, msgtype, flags);
	if (status == -1) {
		if (await) { perror("msgrcv failed"); }
		return false;
	} else {
		printf("received message: %s\n", msgout->text);
		return true;
	}
}

void MessageQueue::Send(long msgtype, const char * msgtext) const {
	Message request(msgtype, msgtext);
	int status = msgsnd(this->_id, &request, GM_MSG_SIZE, NO_FLAGS);
	if (status == -1) {
		perror("msgsnd failed");
	} else {
		printf("sent message(%ld): %s\n", request.type, request.text);
	}
}

