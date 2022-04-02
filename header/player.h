#ifndef PLAYER_H
#define PLAYER_H


#include "color.h"
#include <string>
#include <vector>

class Game;
class AI;

class Player {
private:
	inline static signed char nplayers = 0;

	signed char _id;
	Color _color;

	const Game & _game;
	AI * _ai = nullptr;
	
public:
	Player(const Color &, const Game &);
	~Player(void);
	inline signed char GetId(void) const { return _id; }
	inline const Color & GetColor(void) const { return _color; }
	
	void InitAI(const std::string &);
	void Print(void) const;
	void Run(void);
};

#endif
