#include "player.h"
#include "game.h"
#include "board.h"

#include <cstdio>
#include <chrono>
#include <thread>

Player::Player(const std::string & name, const Color & color) : _id(nplayers++), _name(name), _color(color) {
	this->_seed = std::random_device()();
	this->_generator = std::mt19937(_seed);
	
}

void Player::print(void) const {
	printf("\033[48;2;%d;%d;%dm     \033[0m ", _color.R, _color.G, _color.B);
	printf("PLAYER %d ", _id);
	printf("%s ", _name.c_str());
	printf("(%d,%d,%d)\n", _color.R, _color.G, _color.B);
}

void Player::Run(void) {
	std::uniform_int_distribution<int> dist_x(0, _board->width());
	std::uniform_int_distribution<int> dist_y(0, _board->height());

	for (int i = 0; i < 1000; ++i) {
		_board->Mark(_id, dist_y(_generator), dist_x(_generator));
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(10));
	}
}