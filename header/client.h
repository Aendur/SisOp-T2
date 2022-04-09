#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
private:
	int msgID;
	int shmID;

	void Initialize(void);
	void InitMessageQueue(void);
	void InitSharedMemory(void);

	void Dispose(void);
	void DisposeMessageQueue(void);
	void DisposeSharedMemory(void);

public:
	Client(const std::string &);
	~Client(void);
	
	void RequestConnection(void);
};


#endif

