#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <cstddef>

class SharedMemory {
private:
	int    _key = -1;        // Create / Retrieve / Remove
	int    _id = -1;         // Create / Retrieve / Remove
	size_t _size = 0;        // Fetch
	void*  _addr = nullptr; // Attach / Detach

	void Fetch(void);
	void Attach(void);
	void Detach(void);
public:
	~SharedMemory(void);

	// Requests a new shared memory block with given size
	void Create(int key, size_t size);

	// Requests an existing shared memory block
	void Retrieve(int key);

	// Requests an existing shared memory block to be removed
	void Dispose(void);

	inline int    id(void) const { return _id; }
	inline int    key(void) const { return _key; }
	inline size_t size(void) const { return _size; }
	inline void*  addr(void) const { return _addr; }
};


#endif

