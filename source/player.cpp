#include "player.h"

#include <cstdio>

Player::Player(const std::string & name, const Color & color) : _id(_nplayers++), _name(name), _color(color) {}

void Player::print(void) const {
	printf("\033[48;2;%d;%d;%dm   \033[0m ", _color.R, _color.G, _color.B);
	printf("PLAYER %d ", _id);
	printf("%s ", _name.c_str());
	printf("(%d,%d,%d)\n", _color.R, _color.G, _color.B);
}