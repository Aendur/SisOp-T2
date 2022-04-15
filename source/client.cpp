#include "client.h"
#include "keychain.h"

Client::Client(const char * settings_file) {
	// this->seed = (long) std::random_device()();
	// if (this->seed < 100) this->seed += 100;
	// this->generator = std::mt19937_64(seed);

	this->settings.Load(settings_file);
	
	//this->messenger.Retrieve(KeyChain::GetKey(KEY_MQ_CONNECTION));
	this->sm_board.Retrieve(KeyChain::GetKey(KEY_SM_BOARD));
	this->ss_board_row.Retrieve(KeyChain::GetKey(KEY_SS_BOARD_ROW));
	this->ss_board_col.Retrieve(KeyChain::GetKey(KEY_SS_BOARD_COL));
	this->ss_sync.Retrieve(KeyChain::GetKey(KEY_SS_SYNC));
	this->board = (Board*) sm_board.addr();
}

void Client::Connect(void) {
	ss_sync.Op(GM_SEM_GET_ID, -1, true, GM_NO_DELAY);
	this->player_id = board->GetID();
	printf("received id %d\n", this->player_id);
	ai.Initialize(this->settings.ai_file.c_str(), this->player_id, *this->board);
	ai.Print();
	ss_sync.Op(GM_SEM_WAIT_PLAYERS, 1, true, GM_NO_DELAY);
	// char buf[GM_MSG_SIZE];
	// snprintf(buf, GM_MSG_SIZE, "%ld", this->seed);
	// messenger.Send(GM_CONNECTION_REQ, buf);
	// Message m;
	// if (messenger.Receive(this->seed, true, &m)) {
	// 	//#pragma message "TODO handle player ID"
	// 	//int id = std::stoi(m.text);
	// 	this->playerID = (cell_t) std::stoi(m.text);
	// }
}

void Client::Run(void) {
	if (this->player_id == -1) {
		printf("failed to get id\n");
	} else {
		board->Print();
		printf("waiting for others\n");
		ss_sync.Op(GM_SEM_SYNC_BARRIER, -1, true, GM_NO_DELAY);

		printf("init mainloop\n");
		// INIT MAIN LOOP
		this->MainLoop();
		// END MAIN LOOP
		printf("end mainloop\n");
		ss_sync.Op(GM_SEM_END_GAME, 1, true, GM_NO_DELAY);
	}
}

void Client::MainLoop(void) {
	while(ai.HasMoves()) {
		auto [i, j] = ai.NextMove();
		printf("%d %d\n", i, j);
		if (i >= 0 && j >= 0) {
			// LOCK
			ss_board_row.Op(i, -1, true, GM_NO_DELAY);
			ss_board_col.Op(j, -1, true, GM_NO_DELAY);

			// TRY MARK
			//bool marked = _game->MarkBoard(*this, move.first, move.second);
			bool marked = board->Mark(player_id, i, j);
			ai.Delay();

			// UNLOCK
			ss_board_col.Op(j, 1, true, GM_NO_DELAY);
			ss_board_row.Op(i, 1, true, GM_NO_DELAY);

			// NOTIFY
			ai.ConfirmMove(i, j, marked);
		}
	}
}

//void Player::Print(void) const {
//	printf("\033[48;2;%d;%d;%dm     \033[0m ", _color.R, _color.G, _color.B);
//	printf("PLAYER %d ", _id);
//	printf("(%d,%d,%d)\n", _color.R, _color.G, _color.B);
//	this->_ai->Print();
//}

