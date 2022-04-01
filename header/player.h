#ifndef PLAYER_H
#define PLAYER_H


#include "color.h"
#include <string>
#include <vector>

class Game;
class Board;
class AI;

class Player {
private:
	inline static signed char nplayers = 0;

	signed char _id;
	std::string _name;
	Color _color;

	AI * _ai = nullptr;
	Board * _board = nullptr;
	Game * _game = nullptr;
	
public:
	Player(const std::string &, const Color &);
	~Player(void);
	inline void SetGame(Game* g) { _game = g; }
	inline void SetBoard(Board* b) { _board = b; }
	inline const Color & GetColor(void) const { return _color; }
	
	void InitAI(void);
	void Print(void) const;
	void Run(void);
};

#endif
