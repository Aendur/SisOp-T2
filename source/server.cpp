#include "server.h"
#include <cstring>

Server::Server(const char * settings_file) : messenger("Server") {
	settings.Load(settings_file);

	int size = settings.grid_width * settings.grid_height * sizeof(cell_t);
	messenger.InitMessageQueue(true);
	messenger.InitSharedMemory(size);

	board.Initialize(settings.grid_width, settings.grid_height, messenger.GetAddress());
}

Server::~Server(void) {
	messenger.DisposeSharedMemory(true);
	messenger.DisposeMessageQueue(true);
}

void Server::Run(void) {
	int connected_players = 0;
	int W = settings.grid_width;
	int H = settings.grid_height;
	char buf[GM_MSG_SIZE];

	while (connected_players < settings.num_players) {
		Message m = messenger.AwaitMessage(GM_CONNECTION_REQ);

		long token = std::stol(m.text);
		const Color & C = settings.player_colors[connected_players];
		snprintf(buf, GM_MSG_SIZE, "%-4d %-4d %-4d %-4d %-4d", W, H, C.R, C.G, C.B);
		
		messenger.SendMessage(token, buf);
		++connected_players;
	}
	
	while (connected_players > 0) {
		messenger.AwaitMessage(GM_CONNECTION_END);
		--connected_players;
	}
}
