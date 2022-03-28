#include <iostream>

#include "settings.h"
#include "board.h"
#include "UI.h"

int main (int, char ** argv) {
	Settings::LoadSettings("settings.ini");
	Board board_main(Settings::grid_width, Settings::grid_height);
	board_main.print();

	for (auto & player : Player::players) {
		player.print();
		player.SetBoard(&board_main);
		player.Run();
	}
	board_main.print();

	(void)argv;
	UI ui("test", Settings::GetWindowWidth(), Settings::GetWindowHeight());
	ui.Run(&board_main);

	return 0;
}
