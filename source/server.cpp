#include "server.h"
#include "keychain.h"
#include "UISDL.h"

Server::Server(const char * settings_file) {
	this->seed = std::random_device()();
	this->generator = std::mt19937_64(seed);

	this->settings.Load(settings_file);
	int missing = this->settings.num_players - this->settings.player_colors.size();
	while (missing-- > 0) {
		this->settings.player_colors.push_back(GetRandomColor());
	}

	//size_t size = settings.grid_height * settings.grid_width;
	//this->messenger.Create(KeyChain::GetKey(KEY_MQ_CONNECTION));
	this->sm_board.Create(KeyChain::GetKey(KEY_SM_BOARD), Board::GetSize(this->settings));
	this->ss_board_row.Create(KeyChain::GetKey(KEY_SS_BOARD_ROW), settings.grid_height, 1);
	this->ss_board_col.Create(KeyChain::GetKey(KEY_SS_BOARD_COL), settings.grid_width, 1);
	this->ss_sync.Create(KeyChain::GetKey(KEY_SS_SYNC), GM_SS_SYNC_NSEMS, 0); //settings.num_players);
	this->board = Board::Initialize(this->settings, this->sm_board.addr());


	if (settings.show_ui == true) {
		this->ui = new UISDL("Server", this->settings);
		this->ui->Initialize();
		this->ui->DrawBackground();
		this->ui->DrawBorder();
		this->ui->DrawGrid();
		this->ui->Refresh(16);
	}
}

Server::~Server(void) {
	//messenger.Dispose();
	ss_sync.Dispose();
	ss_board_col.Dispose();
	ss_board_row.Dispose();
	sm_board.Dispose();

	if (this->ui != nullptr) {
		this->ui->Dispose();
		delete this->ui;
		this->ui = nullptr;
	}
}

void Server::Run(void) {
	board->Print();
	
	//cell_t connected_players = 0;
	//char buf[GM_MSG_SIZE];
	//Message m;
	//while (connected_players < settings.num_players) {
	//	if (messenger.Receive(GM_CONNECTION_REQ, true, &m)) {
	//		long token = std::stol(m.text);
	//		snprintf(buf, GM_MSG_SIZE, "%d", connected_players);
	//		
	//		messenger.Send(token, buf);
	//		++connected_players;
	//	}
	//}

	while(board->AddID() < settings.num_players) {
		printf("waiting for players (%d)\n", board->GetID());
		ss_sync.Op(GM_SEM_GET_ID, 1, true, GM_NO_DELAY);
		ss_sync.Op(GM_SEM_WAIT_PLAYERS, -1, true, GM_NO_DELAY);
	}

	printf("sync barrier\n");
	printf("----- press return to start game -----\n");
	getchar();
	ss_sync.Op(GM_SEM_SYNC_BARRIER, settings.num_players, true, GM_NO_DELAY);
	
	//for(int i = 0; i < settings.grid_height / 2 + 5; ++i) { printf("\n\n"); }
	//printf("\033[50;1H");
	// "clear" screen
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");

	bool finished = ss_sync.Op(GM_SEM_END_GAME, -settings.num_players, false, GM_NO_DELAY);
	while (!finished) {
		board->Draw();
		finished = ss_sync.Op(GM_SEM_END_GAME, -settings.num_players, true, 33333L);
	}
	board->Draw();

	/*
	int frame_count = 0;
	while (connected_players > 0) {
		if (this->ui != nullptr) {
			this->ui->HandleInput();
			this->ui->Refresh(32);
			//if (this->ui->Quit()) { connected_players = -1; }
			if (this->ui->Quit()) {
				this->ui->Dispose();
				delete this->ui;
				this->ui = nullptr;
			}
			
			if (++frame_count > 30) {
				frame_count = 0;
				if (messenger.Receive(GM_CONNECTION_END, false, &m)) {
					--connected_players;
				}
			}
		} else {
			if (messenger.Receive(GM_CONNECTION_END, true, &m)) {
				--connected_players;
			}
		}
	}
	*/
}

Color Server::GetRandomColor(void) {
	auto distribution = std::uniform_int_distribution<unsigned char>(0, 0xFF);
	unsigned char R = distribution(this->generator);
	unsigned char G = distribution(this->generator);
	unsigned char B = distribution(this->generator);
	unsigned char A = 255; //distribution(this->generator);
	return Color(R,G,B,A);
}

// void Server::SetNextColor(int player, Color * color) {
// 	if (player < (int) settings.player_colors.size()) {
// 		color->R = settings.player_colors[player].R;
// 		color->G = settings.player_colors[player].G;
// 		color->B = settings.player_colors[player].B;
// 	} else {
// 		auto distribution = std::uniform_int_distribution<unsigned char>(0, 0xFF);
// 		color->R = distribution(this->generator);
// 		color->G = distribution(this->generator);
// 		color->B = distribution(this->generator);
// 	}
// }
