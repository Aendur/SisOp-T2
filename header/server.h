#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server {
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
	Server(const std::string &);
	~Server(void);
	
	void Await(void);
};


#endif

