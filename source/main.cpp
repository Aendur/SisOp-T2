#if defined(SERVER)
#include "server.h"

int main (int, char ** argv) {
	(void)argv;

	Server server("");
	server.Run();


	//std::cout << SEMMSL << std::endl;
	//int semid = semget(123, 5, flag);
	//sleep(60);

	// Game * game = new Game();
	// game->LoadSettings("settings.ini");
	// game->Initialize();
	// game->Run();
	// delete game;
	
	// std::cout << "main return 0" << std::endl;
	return 0;
}
#elif defined(CLIENT)
#include "client.h"
#include <cstdlib>
#include <ctime>

int main (int, char ** argv) {
	(void)argv;
	srand(time(NULL));
	Client client("");
	client.Run();

	return 0;
}
#endif

