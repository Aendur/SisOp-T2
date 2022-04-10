#include "keychain.h"
#include <cstdio>
#include <cstring>

int KeyChain::GetKey(bool print) {
	const char ichi[] = "いっちゃん";
	unsigned short * ichi_vec = (unsigned short*) ichi;
	int n = sizeof(ichi) / sizeof(unsigned short);
	
	unsigned short skey = ichi_vec[0];
	for (int i = 1; i < n; ++i) { skey = skey ^ ichi_vec[i]; }
	skey = ~skey;
	int key = skey << 8;

	if (print) {
		printf("S: %s %lu\n", ichi, strlen(ichi));
		printf("k: 0x%x %d\n", skey, skey);
		printf("K: 0x%x %d\n", key, key);
	}

	return key;
}

