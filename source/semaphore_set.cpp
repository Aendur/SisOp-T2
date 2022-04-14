#include "semaphore_set.h"

#include <stdexcept>
#include <cstring>
#include <sys/sem.h>
#include <errno.h>

void SemaphoreSet::Create(int key, int nsems) {
	this->_id = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0600);

	if (this->_id == -1) {
		this->_key = -1;
		const char * msg = "semget failed";
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		this->_key = key;
		printf("semaphore set initialized KEY=%x ID=%d\n", this->_key, this->_id);
	}
}

void SemaphoreSet::Retrieve(int key) {
	this->_id = semget(key, 0, 0600);

	if (this->_id == -1) {
		this->_key = -1;
		const char * msg = "semget failed";
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		this->_key = key;
		printf("semaphore set retrieved KEY=0x%x ID=%d\n", this->_key, this->_id);
	}
}

void SemaphoreSet::Dispose(void) {
	if (this->_id != -1) {
		int status = semctl(this->_id, 0, IPC_RMID);
		if (status == -1) {
			perror("semctl failed");
		} else {
			printf("semapohre set released KEY=0x%x ID=%d\n", this->_key, this->_id);
			this->_id = -1;
			this->_key = -1;
		}
	}
}

//////////

void SemaphoreSet::Op(unsigned short index, short value) {
	struct sembuf op;
	op.sem_num = index;
	op.sem_op = value;
	op.sem_flg = 0;
	
	int status = semop(this->_id, &op, 1);

	if (status == -1) {
		perror("semop failed");
	} else {
		printf("semop sem %u op: %d\n", index, value);
	}
}
