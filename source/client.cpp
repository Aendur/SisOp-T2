#include "client.h"
#include "keychain.h"

Client::Client(const char * settings_file) {
	// this->seed = (long) std::random_device()();
	// if (this->seed < 100) this->seed += 100;
	// this->generator = std::mt19937_64(seed);

	this->settings.Load(settings_file);
	
	//this->messenger.Retrieve(KeyChain::GetKey(KEY_MQ_CONNECTION));
	this->sm_board.Retrieve(KeyChain::GetKey(KEY_SM_BOARD));
	this->ss_sync.Retrieve(KeyChain::GetKey(KEY_SS_SYNC));
	this->board = (Board*) sm_board.addr();
}

void Client::Connect(void) {
	ss_sync.Op(GM_SEM_GET_ID, -1);
	this->player_id = board->GetID();
	printf("received id %d\n", this->player_id);
	ai.Initialize(this->settings.ai_file.c_str(), this->player_id, *this->board);
	ai.Print();
	ss_sync.Op(GM_SEM_WAIT_PLAYERS, 1);
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
		ss_sync.Op(GM_SEM_SYNC_BARRIER, -1);

		printf("init mainloop\n");
		// INIT MAIN LOOP
		this->MainLoop();
		// END MAIN LOOP
		printf("end mainloop\n");
		ss_sync.Op(GM_SEM_END_GAME, 1);
	}
}

void Client::MainLoop(void) {
	while(ai.HasMoves()) {
		const auto & move = ai.NextMove();
		if (move.first >= 0 && move.second >= 0) {
			// LOCK
			// TRY MARK
			//bool marked = _game->MarkBoard(*this, move.first, move.second);
			bool marked = false;
			ai.Delay();
			// UNLOCK

			ai.ConfirmMove(move.first, move.second, marked);
		}
	}
}

// bool Game::MarkBoard(const Player & p, int i, int j) {
// 	//board_lock.lock();
// 	//board_lock_row[i].lock();
// 	//board_lock_col[j].lock();
// 	board_lock_cell[i * board_main->GetWidth() + j].lock();
// 	bool marked = this->board_main->Mark(p.GetId(), i, j);
// 	p.Delay();
// 	board_lock_cell[i * board_main->GetWidth() + j].unlock();
// 	//board_lock_col[j].unlock();
// 	//board_lock_row[i].unlock();
// 	//board_lock.unlock();

// 	return marked;
// }