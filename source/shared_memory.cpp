#include "shared_memory.h"

#include <stdexcept>
#include <cstring>
#include <sys/shm.h>
#include <errno.h>

#define NO_FLAGS 0


//SharedMemory::SharedMemory(int key) : _key(key) {}
SharedMemory::~SharedMemory(void) {
	this->Detach();
}

void SharedMemory::Create(int key, size_t size) {
	this->_id = shmget(key, size, IPC_CREAT | IPC_EXCL | 0600);

	if (this->_id == -1) {
		this->_key = -1;
		const char * msg = "shmget failed";
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		this->_key = key;
		printf("shared memory initialized KEY=0x%x id=%d\n", this->_key, this->_id);
		this->Fetch();
		this->Attach();
	}
}

void SharedMemory::Retrieve(int key) {
	this->_id = shmget(key, 0, 0600);

	if (this->_id == -1) {
		this->_key = -1;
		const char * msg = "shmget failed";
		perror(msg);
		throw std::runtime_error(msg);
	} else {
		this->_key = key;
		printf("shared memory retrieved KEY=0x%x id=%d\n", this->_key, this->_id);
		this->Fetch();
		this->Attach();
	}
}

void SharedMemory::Fetch(void) {
	if (this->_id == -1) {
		throw std::runtime_error("shared memory not initialized");
	} else {
		shmid_ds info;
		int status = shmctl(this->_id, IPC_STAT, &info);

		if (status == -1) {
			const char * error_msg = "shmctl failed";
			perror(error_msg);
			throw std::runtime_error(error_msg);
		} else {
			this->_size = info.shm_segsz;
			printf("shared memory size=%lu\n", this->_size);
		}
	}
}

void SharedMemory::Attach(void) {
	if (this->_id == -1) {
		throw std::runtime_error("shared memory not initialized");
	} else {
		this->_addr = shmat(this->_id, NULL, NO_FLAGS);

		if (this->_addr == (void*) -1) {
			this->_addr = nullptr;
			const char * error_msg = "shmat failed";
			perror(error_msg);
			throw std::runtime_error(error_msg);
		} else {
			printf("shared memory attached to %p\n", this->_addr);
		}
	}
}

void SharedMemory::Detach(void) {
	if (this->_addr != nullptr) {
		int status = shmdt(this->_addr);
		if (status == -1) {
			perror("shmdt failed");
		} else {
			printf("detached(%d) shared memory from address %p\n", status, this->_addr);
			this->_addr = nullptr;
		}
	}
}

void SharedMemory::Dispose(void) {
	this->Detach();

	if (this->_id != -1) {
		int status = shmctl(this->_id, IPC_RMID, NULL);
		if (status == -1) {
			perror("shmctl failed");
		} else {
			printf("shared memory block released KEY=0x%x ID=%d\n", this->_key, this->_id);
			this->_id = -1;
			this->_key = -1;
		}
	}
}
