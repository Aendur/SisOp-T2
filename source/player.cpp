#include "player.h"
#include "game.h"
#include "board.h"
#include "AI.h"

#include <cstdio>
#include <chrono>
#include <thread>

Player::Player(const std::string & name, const Color & color) : _id(nplayers++), _name(name), _color(color) {}

void Player::Print(void) const {
	printf("\033[48;2;%d;%d;%dm     \033[0m ", _color.R, _color.G, _color.B);
	printf("PLAYER %d ", _id);
	printf("%s ", _name.c_str());
	printf("(%d,%d,%d)\n", _color.R, _color.G, _color.B);
}

void Player::Run(void) {
	//for (int i = 0; i < 1000; ++i) {
	while(_ai->HasMoves()) {
		const auto & move = _ai->NextMove();
		if (move.first >= 0 && move.second >= 0) {
			bool marked = _board->Mark(_id, move.first, move.second);
			_ai->ConfirmMove(move.first, move.second, marked);
		}
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(4));
	}
}

void Player::InitAI(void) {
	this->_ai = new AI(this->_board->width(), this->_board->height(), this->_id);
}

Player::~Player(void) {
	if (this->_ai != nullptr) {
		delete this->_ai;
	}
}