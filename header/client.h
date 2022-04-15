#ifndef CLIENT_H
#define CLIENT_H

#include "settings_client.h"
#include "message_queue.h"
#include "shared_memory.h"
#include "semaphore_set.h"
#include "board.h"
#include <random>

class Client {
private:
	long seed;
	std::mt19937_64 generator;

	SettingsClient settings;
	MessageQueue messenger;
	SharedMemory sm_board;
	SemaphoreSet ss_sync;
	Board * board;

	cell_t playerID = -1;
public:
	Client(const char *);
	// ~Client(void);

	void Connect(void);
	void Run(void);
	void MainLoop(void);
};


#endif

