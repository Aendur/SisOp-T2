#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include "settings.h"

class Player;
class Board;
class UI;

class Game {
private:
	Settings settings;
	std::vector<Player*> players;
	Board * board_main;
	UI * ui;

	void InitBoard(void);
	void InitPlayers(void);
	void InitUI(void);

public:
	//Game(void); : settings(s) {}
	~Game(void);
	void LoadSettings(const std::string &);
	void Initialize(void);
	

	void Run(void);
};


#endif

