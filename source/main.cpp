#include <iostream>

#include "settings.h"
#include "board.h"
#include "UI.h"

//#include <ctime>
//#include <filesystem>
//#include <iostream>

int main (int, char ** argv) {
	//auto wdir = (std::filesystem::current_path()  / argv[0]).parent_path();
	//std::cout << "working directory is " << wdir << std::endl;
	//std::filesystem::current_path(wdir);
	//std::srand(std::time(NULL));

	Settings::LoadSettings("settings.ini");
	Board board_main(Settings::grid_width, Settings::grid_height);
	board_main.print();

	(void)argv;
	UI ui("test", Settings::GetWindowWidth(), Settings::GetWindowHeight());
	ui.Run();

	return 0;
}
