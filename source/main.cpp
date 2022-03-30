// #include <iostream>
// #include <thread>
// #include <chrono>

#include "game.h"

int main (int, char ** argv) {
	(void)argv;

	Game game;
	game.LoadSettings("settings.ini");
	game.Initialize();
	
	//std::cout << "return 0" << std::endl;
	//std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
	return 0;
}
