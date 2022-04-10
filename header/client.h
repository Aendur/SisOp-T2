#ifndef CLIENT_H
#define CLIENT_H

#include "messenger.h"
#include <string>

class Client {
private:
	Messenger messenger;
public:
	Client(const std::string &);
	~Client(void);

	void Run(void);
};


#endif

