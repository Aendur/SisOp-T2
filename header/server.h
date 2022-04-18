#ifndef SERVER_H
#define SERVER_H

#include <random>
#include <thread>

#include "settings/settings.h"
#include "board.h"

class UI;
class Player;
class Server {
private:
	unsigned long long seed;
	std::mt19937_64 generator;

	Settings settings;
	Board* board = nullptr;
	UI* ui = nullptr;

	std::vector<std::thread> player_threads;
	std::vector<Player*> player_objects;

	Color GetRandomColor(void);
	void StartPlayers(void);
	void Watch(void);
	void Finish(void);
	void ShowResults(void) const;
public:
	Server(const char *);
	~Server(void);

	void Run(void);
};


#endif

