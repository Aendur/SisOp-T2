#ifndef KEYCHAIN_H
#define KEYCHAIN_H

#define KEY_MQ_CONNECTION 0
#define KEY_SM_BOARD 0
#define KEY_SS_SYNC 0
#define KEY_SS_BOARD_ROW 1
#define KEY_SS_BOARD_COL 2

class KeyChain {
public:
	//static int GetKey(bool = false);
	static int GetKey(int index);
};

#endif
