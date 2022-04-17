#include "player.h"
#include "sync.h"
#include "UI/UIEmpty.h"
#include "UI/UITerm.h"
#include "UI/UISDL.h"

Player::Player(const char * settings_file, Board * board) {
	this->settings.Load(settings_file);
	this->board = board;
	this->board->Print();

	this->ui = new UIEmpty();
	this->ui->Initialize(this->board);
}

Player::~Player(void) {
	if (this->ui != nullptr) {
		this->ui->Dispose();
		delete this->ui;
		this->ui = nullptr;
	}
}

void Player::Start(void) {
	Sync::Wait(GM_SEM_GET_ID, GM_NO_DELAY); // , -1, true, GM_NO_DELAY);
	this->player_id = board->GetID();
	printf("received id %d\n", this->player_id);
	ai.Initialize(&this->settings, this->player_id, *this->board);
	ai.Print();
	Sync::Notify(GM_SEM_WAIT_PLAYERS); //, 1, true, GM_NO_DELAY);

	this->Run();
}

void Player::Run(void) {
	if (this->player_id == -1) {
		printf("failed to get id\n");
	} else {
		printf("waiting for others\n");
		Sync::Wait(GM_SEM_SYNC_BARRIER, GM_NO_DELAY); //, -1, true, GM_NO_DELAY);

		this->Mainloop();
		this->ui->Refresh(0);

		// Sync::ss_sync.Op(GM_SEM_END_GAME, 1, true, GM_NO_DELAY);
		// this->Watch();
		// this->ShowResults();
	}
}

void Player::Mainloop(void) {
	this->ui->Clear();
	while(ai.HasMoves()) {
		auto [i, j] = ai.NextMove();
		if (i >= 0 && j >= 0) {
			// LOCK
			Sync::LockBoard(i, j); //, -1, true, GM_NO_DELAY);

			// TRY MARK
			bool marked = board->Mark(player_id, i, j);
			this->Await();

			// UNLOCK
			Sync::UnlockBoard(i, j); //, 1, true, GM_NO_DELAY);

			// NOTIFY
			ai.ConfirmMove(i, j, marked);
		}
	}
}

void Player::Await(void) {
	ai.Delay();
}
