#ifndef SERVER_H
#define SERVER_H

#include "settings.h"
#include "messenger.h"
#include "board.h"
#include <random>

class UI;
class Server {
private:
	unsigned long long seed;
	std::mt19937_64 generator;

	Settings settings;
	Messenger messenger;
	Board board;

	UI * ui = nullptr;

	void SetNextColor(int, Color *);
public:
	Server(const char *);
	~Server(void);

	void Run(void);
};


#endif

