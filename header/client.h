#ifndef CLIENT_H
#define CLIENT_H

#include "shared_memory.h"
#include "semaphore_set.h"
#include "board.h"
#include "ai.h"

class UI;
class Client {
private:
	SettingsAI settings;
	SharedMemory sm_board;
	SemaphoreSet ss_board_row;
	SemaphoreSet ss_board_col;
	SemaphoreSet ss_sync;
	Board* board = nullptr;
	UI* ui = nullptr;

	cell_t player_id = -1;
	AI ai;

	void Mainloop(void);
	void Watch(void);
	void Await(void);
	void ShowResults(void) const;
public:
	Client(const char *);
	~Client(void);

	void Connect(void);
	void Run(void);
};


#endif

