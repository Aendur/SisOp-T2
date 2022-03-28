#include "player.h"
#include "board.h"

#include <cstdio>

void Player::AddPlayer(const std::string & name, const Color & color) {
	unsigned long long seed = std::random_device()();
	players.push_back(Player(name, color).BuildRNG(seed));

}

Player & Player::BuildRNG(unsigned long long seed) {
	this->_seed = seed;
	this->_generator = std::mt19937(seed);
	return *this;
}

Player::Player(const std::string & name, const Color & color) : _id(players.size()), _name(name), _color(color) {}

void Player::print(void) const {
	printf("\033[48;2;%d;%d;%dm   \033[0m ", _color.R, _color.G, _color.B);
	printf("PLAYER %d ", _id);
	printf("%s ", _name.c_str());
	printf("(%d,%d,%d)\n", _color.R, _color.G, _color.B);
}

void Player::Run(void) {
	std::uniform_int_distribution<int> dist_x(0, _board->width());
	std::uniform_int_distribution<int> dist_y(0, _board->height());

	for (int i = 0; i < 100; ++i) {
		_board->Mark(_id, dist_y(_generator), dist_x(_generator));
	}
}