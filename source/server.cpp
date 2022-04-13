#include "server.h"
#include "keychain.h"
#include "UISDL.h"


Server::Server(const char * settings_file) : messenger("Server") {
	this->seed = std::random_device()();
	this->generator = std::mt19937_64(seed);

	this->settings.Load(settings_file);
	int missing = this->settings.num_players - this->settings.player_colors.size();
	while (missing-- > 0) {
		this->settings.player_colors.push_back(GetRandomColor());
	}

	//int size = settings.grid_width * settings.grid_height * sizeof(cell_t);
	//messenger.InitMessageQueue(true);
	//messenger.InitSharedMemory(size);
	this->mblock.Create(KeyChain::GetKey(SHMKEY_BOARD), Board::GetSize(this->settings));
	this->board = Board::Initialize(this->settings, this->mblock.addr());

	//board.Initialize(settings.grid_width, settings.grid_height, mblock.GetAddress());

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
	//messenger.DisposeSharedMemory(true);
	//messenger.DisposeMessageQueue(true);
	mblock.Dispose();

	if (this->ui != nullptr) {
		this->ui->Dispose();
		delete this->ui;
		this->ui = nullptr;
	}
}

void Server::Run(void) {
	board->Print();
	/*int connected_players = 0;
	int W = settings.grid_width;
	int H = settings.grid_height;
	Color C(0,0,0,255);
	char buf[GM_MSG_SIZE];

	Message m;
	while (connected_players < settings.num_players) {
		if (messenger.AwaitMessage(GM_CONNECTION_REQ, true, &m)) {
			long token = std::stol(m.text);
			this->SetNextColor(connected_players, &C);
			snprintf(buf, GM_MSG_SIZE, "%-4d %-4d %-4d %-4d %-4d", W, H, C.R, C.G, C.B);
			
			messenger.SendMessage(token, buf);
			++connected_players;
		}
	}
	
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
				if (messenger.AwaitMessage(GM_CONNECTION_END, false, &m)) {
					--connected_players;
				}
			}
		} else {
			if (messenger.AwaitMessage(GM_CONNECTION_END, true, &m)) {
				--connected_players;
			}
		}
	}*/
}

Color Server::GetRandomColor(void) {
	auto distribution = std::uniform_int_distribution<unsigned char>(0, 0xFF);
	unsigned char R = distribution(this->generator);
	unsigned char G = distribution(this->generator);
	unsigned char B = distribution(this->generator);
	unsigned char A = 255; //distribution(this->generator);
	return Color(R,G,B,A);
}

void Server::SetNextColor(int player, Color * color) {
	if (player < (int) settings.player_colors.size()) {
		color->R = settings.player_colors[player].R;
		color->G = settings.player_colors[player].G;
		color->B = settings.player_colors[player].B;
	} else {
		auto distribution = std::uniform_int_distribution<unsigned char>(0, 0xFF);
		color->R = distribution(this->generator);
		color->G = distribution(this->generator);
		color->B = distribution(this->generator);
	}
}
