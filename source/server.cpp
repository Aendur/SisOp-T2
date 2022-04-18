#include "server.h"
#include "sync.h"
#include "UI/UIEmpty.h"
#include "UI/UITerm.h"
#include "UI/UISDL.h"

#include "player.h"

Server::Server(const char * settings_file) {
	this->seed = std::random_device()();
	this->generator = std::mt19937_64(seed);

	this->settings.Load(settings_file);

	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	printf("%d\n", settings.show_ui);
	int missing = this->settings.num_players - this->settings.player_colors.size();
	while (missing-- > 0) {
		this->settings.player_colors.push_back(GetRandomColor());
	}

	Sync::Initialize(this->settings);
	
	this->board = new Board(this->settings); //, this->sm_board.addr());
	this->board->Print();

	for (int i = 0; i < settings.num_players; ++i) {
		Player * new_player = new Player("settings_client.ini", this->board);
		player_objects.push_back(new_player);
	}


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
	Sync::Dispose();

	delete this->board;
	
	for(Player * client : this->player_objects) { delete client; }
	this->player_objects.clear();

	if (this->ui != nullptr) {
		this->ui->Dispose();
		delete this->ui;
		this->ui = nullptr;
	}
}

void Server::Run(void) {
	this->StartPlayers();
	this->Watch();
	this->Finish();
	this->ShowResults();
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

void Server::StartPlayers(void) {
	printf("initializing players\n");
	for (Player * player : this->player_objects) {
		player_threads.emplace_back(&Player::Start, player);
	}
}

void Server::Watch(void) {
	this->ui->Clear();
	int remaining_players = settings.num_players;
	do {
		this->ui->Refresh(0);
		//for (auto & thread : player_threads) {
		//	if (thread.joinable()) {
		//		thread.join();
		//		--remaining_players;
		//	}
		//}
	} while (remaining_players > 0); //while (!Sync::ss_sync.Op(GM_SEM_END_GAME, -settings.num_players, true, 33333L));
	this->ui->Refresh(10);
}

void Server::Finish(void) {
	printf("\n\n\ngame finished\n");
}


void Server::ShowResults(void) const {
	printf("\n\n\n     ----- RESULTS -----\n\n");
	for (const auto & [score,player] : board->CountScores()) {
		const Color & c = board->GetColor(player);
		printf("     \033[48;2;%d;%d;%dm   \033[0m Player %d - Score: %d\n", c.R, c.G, c.B, player, score);
	}
	printf("\n\n");
	
	if (settings.wait_for_input) {
		printf("----- press return to exit -----\n");
		getchar();
	}
}
