#include <iostream>
// #include "game.h"

#include "client.h"
#include <cstdlib>
#include <ctime>

int main (int, char ** argv) {
	(void)argv;
	srand(time(NULL));
	Client client("");
	client.RequestConnection();

	return 0;
}
