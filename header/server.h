#ifndef SERVER_H
#define SERVER_H

#include "messenger.h"
#include <string>

class Server {
private:
	Messenger messenger;

public:
	Server(const std::string &);
	~Server(void);
	
	void Run(void);
};


#endif

