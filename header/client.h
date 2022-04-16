#ifndef CLIENT_H
#define CLIENT_H

#include "settings/settings_client.h"
#include "shared_memory.h"
#include "semaphore_set.h"
#include "board.h"
#include "ai.h"

class UI;
class Client {
private:
	// long seed;
	// std::mt19937_64 generator;

	SettingsClient settings;
	//MessageQueue messenger;
	SharedMemory sm_board;
	SemaphoreSet ss_board_row;
	SemaphoreSet ss_board_col;
	SemaphoreSet ss_sync;
	Board* board = nullptr;
	UI* ui = nullptr;

	cell_t player_id = -1;
	AI ai;

	void Mainloop(void);
	void Move(void);
	void Watch(void);
public:
	Client(const char *);
	~Client(void);

	void Connect(void);
	void Run(void);
};


#endif

