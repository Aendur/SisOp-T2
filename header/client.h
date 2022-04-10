#ifndef CLIENT_H
#define CLIENT_H

//#include "settings.h"
#include "messenger.h"
#include "board.h"

#include <random>


class Client {
private:
	//Settings settings;
	long seed;
	std::mt19937_64 generator;

	Messenger messenger;
	Board board;
	
public:
	Client(const char *);
	~Client(void);

	void Connect(void);
	void Run(void);
};


#endif

