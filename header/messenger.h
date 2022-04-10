#ifndef MESSENGER_H
#define MESSENGER_H

#include "message.h"
#include <string>

class Messenger {
private:
	const std::string speaker_tag;

	int msgID = -1;
	int shmID = -1;
	char * shmAT = nullptr;
	int NBYTES = 0;

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
	
	const Message AwaitMessage(long msgtype) const;
	void SendMessage(long msgtype, const char * msgtext) const;
	void ReadData(void);
	void WriteData(void);
};


#endif

