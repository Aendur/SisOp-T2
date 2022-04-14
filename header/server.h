#ifndef SERVER_H
#define SERVER_H

#include <random>

#include "settings.h"
#include "message_queue.h"
#include "shared_memory.h"
#include "semaphore_set.h"
#include "board.h"

class UI;
class Server {
private:
	unsigned long long seed;
	std::mt19937_64 generator;

	Settings settings;
	MessageQueue messenger;
	SharedMemory mblock;
	SemaphoreSet ssync;
	Board* board;

	UI * ui = nullptr;

	//void SetNextColor(int, Color *);
	Color GetRandomColor(void);
public:
	Server(const char *);
	~Server(void);

	void Run(void);
};


#endif

