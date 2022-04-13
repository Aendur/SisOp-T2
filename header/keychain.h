#ifndef KEYCHAIN_H
#define KEYCHAIN_H

#define SHMKEY_BOARD 0

class KeyChain {
public:
	//static int GetKey(bool = false);
	static int GetKey(int index);
};

#endif
