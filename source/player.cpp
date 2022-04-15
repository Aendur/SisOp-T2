#include "player.h"
#include "game.h"
#include "board.h"
#include "AI.h"

#include <cstdio>

Player::Player(const Color & color, Game * game) : _id(nplayers++), _color(color), _game(game) {}

void Player::Print(void) const {
	printf("\033[48;2;%d;%d;%dm     \033[0m ", _color.R, _color.G, _color.B);
	printf("PLAYER %d ", _id);
	printf("(%d,%d,%d)\n", _color.R, _color.G, _color.B);
	this->_ai->Print();
}

void Player::Run(void) {
	while(_ai->HasMoves()) {
		const auto & move = _ai->NextMove();
		if (move.first >= 0 && move.second >= 0) {
			bool marked = _game->MarkBoard(*this, move.first, move.second);
			_ai->ConfirmMove(move.first, move.second, marked);
		}
		//_ai->Delay();
	}
	_game->NotifyFinished();
}

void Player::InitAI(const std::string & path) {
	//this->_ai = new AI(path.c_str(), *this, this->_game->GetBoard());
}

Player::~Player(void) {
	if (this->_ai != nullptr) {
		delete this->_ai;
	}
}

void Player::Delay(void) const {
	_ai->Delay();
}