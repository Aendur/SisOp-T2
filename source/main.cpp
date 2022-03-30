#include <iostream>

#include <thread>
#include <chrono>

#include "settings.h"
#include "board.h"
#include "UI.h"

int main (int, char ** argv) {
	(void)argv;
	
	Settings * settings = new Settings();
	settings->Load("settings.ini");

	/*
	Board board_main(Settings::grid_width, Settings::grid_height);
	board_main.print();

	for (auto & player : Player::players) {
		player.print();
		player.SetBoard(&board_main);
		player.Run();
	}
	board_main.print();
	*/

	UI * ui = new UI("test", *settings);
	ui->Initialize();

	SDL_Delay(5000);

	ui->Dispose();
	//ui.Run(&board_main);
	delete ui;

	delete settings;
	std::cout << "return 0" << std::endl;
	std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
	return 0;
}
