#include "player.h"
#include "sync.h"
#include "UI/UIEmpty.h"
#include "UI/UITerm.h"
#include "UI/UISDL.h"

Player::Player(const char * settings_file, Board * board) {
	this->settings.Load(settings_file);
	this->board = board;
	//this->board->Print();
}

// Player::~Player(void) { }

void Player::Start(void) {
	Sync::LockSync(GM_SEM_GET_ID);
	this->player_id = board->AddID();
	Sync::UnlockSync(GM_SEM_GET_ID);

	ai.Initialize(&this->settings, this->player_id, *this->board);
	//ai.Print();
	//printf("received id %d\nwaiting for others\n", this->player_id);

	Sync::Wait(GM_SEM_GET_ID, GM_NO_DELAY, [this](void) { return board->GetID() >= board->GetNPlayers() - 1; });
	Sync::Broadcast(GM_SEM_GET_ID);
	this->Run();
}

void Player::Run(void) {
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
