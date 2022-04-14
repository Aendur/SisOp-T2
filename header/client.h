#ifndef CLIENT_H
#define CLIENT_H

//#include "settings.h"
#include "message_queue.h"
#include "board.h"
#include <random>

class Client {
private:
	long seed;
	std::mt19937_64 generator;

	//Settings settings;
	MessageQueue messenger;
	Board board;
public:
	Client(const char *);
	~Client(void);

	void Connect(void);
	void Run(void);
};


#endif

