#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "message.h"
#include <string>

class MessageQueue {
private:
	int _key = -1;
	int _id = -1;

public:
	//~Messenger(void);

	void Create(int key);
	void Retrieve(int key);
	void Dispose(void);

	bool Receive(long msgtype, bool await, Message * out) const;
	void Send(long msgtype, const char * msgtext) const;
};


#endif

