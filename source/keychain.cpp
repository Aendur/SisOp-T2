#include "keychain.h"
#include <cstdio>
#include <cstring>

const char * ichi = "いっちゃん";

int KeyChain::GetKey(bool print) {
	unsigned long long * iichi = (unsigned long long*) ichi;
	int  q1 = (iichi[1] & 0x00000000FFFFFFFFLL);
	int  q2 = (iichi[1] & 0xFFFFFFFF00000000LL) >> 32;
	int  q3 = (iichi[0] & 0x00000000FFFFFFFFLL);
	int  q4 = (iichi[0] & 0xFFFFFFFF00000000LL) >> 32;
	int key = ~(q1 ^ q2 ^ q3 ^ q4);

	if (print) {
		printf("S: %s %lu\n", ichi, strlen(ichi));
		printf("4: 0x%x\n3: 0x%x\n", q4, q3);
		printf("2: 0x%x\n1: 0x%x\n", q2, q1);
		printf("K: 0x%x %d\n", key, key);
	}

	return key;
}

