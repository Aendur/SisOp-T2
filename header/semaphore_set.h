#ifndef SEMAPHORE_SET_H
#define SEMAPHORE_SET_H


class SemaphoreSet {
private:
	int    _key = -1;        // Create / Retrieve / Remove
	int    _id = -1;         // Create / Retrieve / Remove
	unsigned long _nsems = 0;        // Fetch

	void Fetch(void);
	void Initialize(int init_value);

public:
	// Requests a new shared memory block with given size
	void Create(int key, int nsems, int init_value);

	// Requests an existing shared memory block
	void Retrieve(int key);

	// Requests an existing shared memory block to be removed
	void Dispose(void);

	void Op(unsigned short index, short value);
	void Wait(void);
	void Post(void);
};


#endif
