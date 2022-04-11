#ifndef MESSENGER_H
#define MESSENGER_H

#include "message.h"
#include <string>

class Messenger {
private:
	const std::string speaker_tag;

	int msgID = -1;
	int shmID = -1;
	int NBYTES = 0;
	void * shmAT = nullptr;

	const char * Tag(const char * msg) const;
	
	void RetrieveSharedMemory(void);
	void AttachSharedMemory(void);
	void DetachSharedMemory(void);

public:
	Messenger(const std::string &);
	//~Messenger(void);

	void InitMessageQueue(bool create_new);
	void InitSharedMemory(int create_new_size);
	void DisposeSharedMemory(bool remove);
	void DisposeMessageQueue(bool remove);
	
	bool AwaitMessage(long msgtype, bool await, Message * out) const;
	void SendMessage(long msgtype, const char * msgtext) const;

	inline void * GetAddress(void) const { return shmAT; }
};


#endif

