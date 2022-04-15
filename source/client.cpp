#include "client.h"
#include "keychain.h"

Client::Client(const char * settings_file) {
	this->seed = (long) std::random_device()();
	if (this->seed < 100) this->seed += 100;
	this->generator = std::mt19937_64(seed);

	this->settings.Load(settings_file);
	
	this->messenger.Retrieve(KeyChain::GetKey(KEY_MQ_CONNECTION));
	this->sm_board.Retrieve(KeyChain::GetKey(KEY_SM_BOARD));
	this->ss_sync.Retrieve(KeyChain::GetKey(KEY_SS_SYNC));
	this->board = (Board*) sm_board.addr();
}

void Client::Connect(void) {
	char buf[GM_MSG_SIZE];
	snprintf(buf, GM_MSG_SIZE, "%ld", this->seed);
	messenger.Send(GM_CONNECTION_REQ, buf);
	
	Message m;
	if (messenger.Receive(this->seed, true, &m)) {
		//#pragma message "TODO handle player ID"
		//int id = std::stoi(m.text);
		this->playerID = (cell_t) std::stoi(m.text);
	}
}

// #include <thread>
// #include <chrono>
void Client::Run(void) {
	if (this->playerID == -1) {
		printf("failed to get id\n");
	} else {
		//std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(30000));
		// getchar();
		// messenger.Send(GM_CONNECTION_END, "OK");
		board->Print();
		printf("waiting for others\n");
		ss_sync.Op(0,  1);
		printf("sync barrier\n");
		ss_sync.Op(1, -1);

		printf("init mainloop\n");
		// INIT MAIN LOOP
		this->MainLoop();
		// END MAIN LOOP
		printf("end mainloop\n");
		ss_sync.Op(2, 1);
	}
}

void Client::MainLoop(void) {
	// while(_ai->HasMoves()) {
	// 	const auto & move = _ai->NextMove();
	// 	if (move.first >= 0 && move.second >= 0) {
	// 		bool marked = _game->MarkBoard(*this, move.first, move.second);
	// 		_ai->ConfirmMove(move.first, move.second, marked);
	// 	}
	// }
}