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
	signed char _id;
	std::string _name;
	Color _color;
	
	unsigned long long _seed;
	std::mt19937  _generator;
	Board * _board = nullptr;
	Game * _game = nullptr;
	
public:
	inline static std::vector<Player> players = std::vector<Player>();
	static void AddPlayer(const std::string &, const Color &);

	Player(const std::string &, const Color &);
	inline void SetGame(Game* g) { _game = g; }
	inline void SetBoard(Board* b) { _board = b; }
	inline const Color & color(void) const { return _color; }
	void print(void) const;
	void Run(void);
};

#endif
