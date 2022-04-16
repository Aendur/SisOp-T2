#ifndef SERVER_H
#define SERVER_H

#include <random>

#include "settings/settings.h"
#include "shared_memory.h"
#include "semaphore_set.h"
#include "board.h"

class UI;
class Server {
private:
	unsigned long long seed;
	std::mt19937_64 generator;

	Settings settings;
	SharedMemory sm_board;
	SemaphoreSet ss_board_row;
	SemaphoreSet ss_board_col;
	SemaphoreSet ss_sync;
	Board* board = nullptr;
	UI* ui = nullptr;

	Color GetRandomColor(void);
	void Connect(void);
	void Sync(void);
	void Watch(void);
	void Finish(void);
	void ShowResults(void) const;
public:
	Server(const char *);
	~Server(void);

	void Run(void);
};


#endif

