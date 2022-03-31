#ifndef PLAYER_H
#define PLAYER_H


#include "color.h"
#include <string>
#include <vector>
#include <random>

class Game;
class Board;

class Player {
private:
	inline static signed char nplayers = 0;

	signed char _id;
	std::string _name;
	Color _color;
	
	unsigned long long _seed;
	std::mt19937  _generator;
	Board * _board = nullptr;
	Game * _game = nullptr;
	
public:
	Player(const std::string &, const Color &);
	inline void SetGame(Game* g) { _game = g; }
	inline void SetBoard(Board* b) { _board = b; }
	inline const Color & color(void) const { return _color; }
	void print(void) const;
	void Run(void);
};

#endif
