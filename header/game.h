#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <mutex>
#include "settings.h"

class Player;
class Board;
class UI;

class Game {
private:
	std::mutex board_lock;
	std::vector<std::mutex> board_lock_col;
	std::vector<std::mutex> board_lock_row;
	std::vector<std::mutex> board_lock_cell;

	Settings settings;
	std::vector<Player*> players;
	Board * board_main;
	UI * ui;

	void InitBoard(void);
	void InitPlayers(void);
	void InitUI(void);

	void RedrawUI(void) const;

public:
	//Game(void); : settings(s) {}
	~Game(void);
	void LoadSettings(const std::string &);
	void Initialize(void);
	void Run(void);

	bool MarkBoard(const Player & p, int i, int j);

	inline const Board & GetBoard(void) const { return *board_main; }
};

#endif
