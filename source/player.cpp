#include "player.h"

Player::Player(const std::string & name, const Color & color) : _id(_nplayers++), _name(name), _color(color) {}

