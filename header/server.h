#ifndef SERVER_H
#define SERVER_H

#include "settings.h"
#include "shared_memory.h"
#include "board.h"
#include "messenger.h"
#include <random>

class UI;
class Server {
private:
	unsigned long long seed;
	std::mt19937_64 generator;

	Settings settings;
	Messenger messenger;
	SharedMemory mblock;
	Board* board;

	UI * ui = nullptr;

	void SetNextColor(int, Color *);
	Color GetRandomColor(void);
public:
	Server(const char *);
	~Server(void);

	void Run(void);
};


#endif

