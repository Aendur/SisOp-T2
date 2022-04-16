#include "server.h"
#include "keychain.h"
#include "UI/UIEmpty.h"
#include "UI/UITerm.h"
#include "UI/UISDL.h"

Server::Server(const char * settings_file) {
	this->seed = std::random_device()();
	this->generator = std::mt19937_64(seed);

	this->settings.Load(settings_file);
	int missing = this->settings.num_players - this->settings.player_colors.size();
	while (missing-- > 0) {
		this->settings.player_colors.push_back(GetRandomColor());
	}

	this->sm_board.Create(KeyChain::GetKey(KEY_SM_BOARD), Board::GetSize(this->settings));
	this->ss_board_row.Create(KeyChain::GetKey(KEY_SS_BOARD_ROW), settings.grid_height, 1);
	this->ss_board_col.Create(KeyChain::GetKey(KEY_SS_BOARD_COL), settings.grid_width, 1);
	this->ss_sync.Create(KeyChain::GetKey(KEY_SS_SYNC), GM_SS_SYNC_NSEMS, 0); //settings.num_players);
	this->board = Board::Initialize(this->settings, this->sm_board.addr());
	this->board->Print();

	switch (settings.show_ui) {
		case GM_UI_EMPTY:
			this->ui = new UIEmpty();
			break;
		case GM_UI_TERMINAL:
			this->ui = new UITerm(this->settings);
			break;
		case GM_UI_SDL:
			this->ui = new UISDL("Server", this->settings);
			break;
		default:
			printf("unrecognized UI option - fallback to terminal UI\n");
			this->ui = new UITerm(this->settings);
			break;
	}
	this->ui->Initialize(this->board);
}

Server::~Server(void) {
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
	this->Connect();
	this->Sync();
	this->Watch();
	this->Finish();
}

////////////////

Color Server::GetRandomColor(void) {
	auto distribution = std::uniform_int_distribution<unsigned char>(0, 0xFF);
	unsigned char R = distribution(this->generator);
	unsigned char G = distribution(this->generator);
	unsigned char B = distribution(this->generator);
	unsigned char A = 255;
	return Color(R,G,B,A);
}

void Server::Connect(void) {
	while(board->AddID() < settings.num_players) {
		printf("waiting for players (%d)\n", board->GetID());
		ss_sync.Op(GM_SEM_GET_ID, 1, true, GM_NO_DELAY);
		ss_sync.Op(GM_SEM_WAIT_PLAYERS, -1, true, GM_NO_DELAY);
	}
}

void Server::Sync(void) {
	printf("sync barrier\n");
	if (settings.wait_for_input) {
		printf("----- press return to start game -----\n");
		getchar();
	}
	ss_sync.Op(GM_SEM_SYNC_BARRIER, settings.num_players, true, GM_NO_DELAY);
}

void Server::Watch(void) {
	this->ui->Clear();
	do {
		this->ui->Refresh(0);
	} while (!ss_sync.Op(GM_SEM_END_GAME, -settings.num_players, true, 33333L));
	this->ui->Refresh(10);
}

void Server::Finish(void) {
	printf("\n\n\ngame finished\n");
	if (settings.wait_for_input) {
		printf("----- press return to exit -----\n");
		getchar();
	}
	ss_sync.Op(GM_SEM_SYNC_BARRIER, settings.num_players, true, GM_NO_DELAY);
}
