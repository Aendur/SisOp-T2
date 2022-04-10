#ifndef SERVER_H
#define SERVER_H

#include "settings.h"
#include "messenger.h"
#include "board.h"

class Server {
private:
	Settings settings;
	Messenger messenger;
	Board board;
public:
	Server(const char *);
	~Server(void);
	
	void Run(void);
};


#endif

