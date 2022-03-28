#ifndef PLAYER_H
#define PLAYER_H


#include "color.h"
#include <string>

class Player {
private:
	inline static int _nplayers = 0;
	int _id;
	std::string _name;
	Color _color;
	
	unsigned char * _board = nullptr;
public:
	Player(const std::string &, const Color &);
};

#endif
