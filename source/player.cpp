#include "player.h"
#include "game.h"
#include "board.h"
#include "AI.h"

#include <cstdio>
#include <chrono>
#include <thread>

Player::Player(const Color & color, const Game & game) : _id(nplayers++), _color(color), _game(game) {}

void Player::Print(void) const {
	printf("\033[48;2;%d;%d;%dm     \033[0m ", _color.R, _color.G, _color.B);
	printf("PLAYER %d ", _id);
	printf("(%d,%d,%d)\n", _color.R, _color.G, _color.B);
}

void Player::Run(void) {
	while(_ai->HasMoves()) {
		const auto & move = _ai->NextMove();
		if (move.first >= 0 && move.second >= 0) {
			bool marked = _game.GetBoard().Mark(_id, move.first, move.second);
			_ai->ConfirmMove(move.first, move.second, marked);
		}
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(4));
	}
}

void Player::InitAI(const std::string & path) {
	this->_ai = new AI(path, this->_game.GetBoard());
}

Player::~Player(void) {
	if (this->_ai != nullptr) {
		delete this->_ai;
	}
}