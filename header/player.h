#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include "ai.h"

class UI;
class Player {
private:
	SettingsAI settings;
	Board* board = nullptr;
	UI* ui = nullptr;

	cell_t player_id = -1;
	AI ai;

	void Run(void);
	void Mainloop(void);
	void Await(void);
public:
	Player(const char *, Board*);
	~Player(void);

	void Start(void);
};


#endif

