#include <iostream>
#include "game.h"

int main (int, char ** argv) {
	(void)argv;

	Game game;
	game.LoadSettings("settings.ini");
	game.Initialize();
	game.Run();
	
	std::cout << "main return 0" << std::endl;
	return 0;
}
