#include "semaphore_set.h"

#include <stdexcept>
#include <cstring>
#include <sys/sem.h>
#include <errno.h>

void SemaphoreSet::Create(int key, int nsems, int init_value) {
	this->_id = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0600);

	if (this->_id == -1) {
		this->_key = -1;
		const char * msg = "semget failed";
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		this->_key = key;
		printf("semaphore set initialized KEY=%x ID=%d\n", this->_key, this->_id);
		this->Fetch();
		this->Initialize(init_value);
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
		this->Fetch();
	}
}

// from MAN pages
union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

void SemaphoreSet::Fetch(void) {
	if (this->_id == -1) {
		throw std::runtime_error("semaphore set not initialized");
	} else {
		struct semid_ds info;
		union semun arg;
		arg.buf = &info;
		int status = semctl(this->_id, 0, IPC_STAT, arg);

		if (status == -1) {
			const char * error_msg = "semctl failed";
			perror(error_msg);
			throw std::runtime_error(error_msg);
		} else {
			this->_nsems = info.sem_nsems;
			printf("sempahore set size=%lu\n", this->_nsems);
		}
	}
}

void SemaphoreSet::Initialize(int init_value) {
	if (this->_id == -1) {
		throw std::runtime_error("semaphore set not initialized");
	} else {
		union semun arg;
		arg.array = new unsigned short[_nsems];
		for (unsigned long i = 0; i < _nsems; ++i) { arg.array[i] = (unsigned short) init_value; }
		int status = semctl(this->_id, 0, SETALL, arg);
		delete[] arg.array;

		if (status == -1) {
			const char * error_msg = "semctl failed";
			perror(error_msg);
			throw std::runtime_error(error_msg);
		} else {
			printf("sempahores initialized to %d\n", init_value);
		}
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
