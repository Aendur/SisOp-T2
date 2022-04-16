#include "client.h"
#include "keychain.h"
#include "UI/UIEmpty.h"
#include "UI/UITerm.h"
#include "UI/UISDL.h"

Client::Client(const char * settings_file) {
	this->settings.Load(settings_file);
	
	this->sm_board.Retrieve(KeyChain::GetKey(KEY_SM_BOARD));
	this->ss_board_row.Retrieve(KeyChain::GetKey(KEY_SS_BOARD_ROW));
	this->ss_board_col.Retrieve(KeyChain::GetKey(KEY_SS_BOARD_COL));
	this->ss_sync.Retrieve(KeyChain::GetKey(KEY_SS_SYNC));
	this->board = (Board*) sm_board.addr();
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

Client::~Client(void) {
	if (this->ui != nullptr) {
		this->ui->Dispose();
		delete this->ui;
		this->ui = nullptr;
	}
}

void Client::Connect(void) {
	ss_sync.Op(GM_SEM_GET_ID, -1, true, GM_NO_DELAY);
	this->player_id = board->GetID();
	printf("received id %d\n", this->player_id);
	ai.Initialize(this->settings.ai_file.c_str(), this->player_id, *this->board);
	ai.Print();
	ss_sync.Op(GM_SEM_WAIT_PLAYERS, 1, true, GM_NO_DELAY);
}

void Client::Run(void) {
	if (this->player_id == -1) {
		printf("failed to get id\n");
	} else {
		printf("waiting for others\n");
		ss_sync.Op(GM_SEM_SYNC_BARRIER, -1, true, GM_NO_DELAY);

		printf("init mainloop\n");
		this->Mainloop();
		this->ui->Refresh(0);

		printf("end mainloop\n");
		ss_sync.Op(GM_SEM_END_GAME, 1, true, GM_NO_DELAY);

		printf("waiting for game to finish\n");
		do {
			this->ui->Refresh(0);
		} while (!ss_sync.Op(GM_SEM_SYNC_BARRIER, -1, true, 33333L));
		this->ui->Refresh(0);
		printf("\n\n\ngame finished\n");
	}
}

void Client::Mainloop(void) {
	this->ui->Clear();
	while(ai.HasMoves()) {
		auto [i, j] = ai.NextMove();
		if (i >= 0 && j >= 0) {
			// LOCK
			ss_board_row.Op(i, -1, true, GM_NO_DELAY);
			ss_board_col.Op(j, -1, true, GM_NO_DELAY);

			// TRY MARK
			bool marked = board->Mark(player_id, i, j);
			this->Watch();

			// UNLOCK
			ss_board_col.Op(j, 1, true, GM_NO_DELAY);
			ss_board_row.Op(i, 1, true, GM_NO_DELAY);

			// NOTIFY
			ai.ConfirmMove(i, j, marked);
		}
	}
}


void Client::Watch(void) {
	static const long long ai_delay = ai.GetDelay();
	static const long long frame_delay = 33333L;
	static long long elapsed = 0L;

	if (frame_delay < ai_delay) {
		while(elapsed + frame_delay < ai_delay) {
			elapsed += frame_delay;
			this->ui->Refresh(frame_delay / 1000L);
		}
		ai.Delay(ai_delay - elapsed);
		elapsed = 0L;
	} else {
		ai.Delay();
		elapsed += ai_delay;
		if (elapsed > frame_delay) {
			this->ui->Refresh(0);
			elapsed = 0L;
		}
	}
}

