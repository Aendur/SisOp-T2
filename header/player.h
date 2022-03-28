#ifndef PLAYER_H
#define PLAYER_H


#include "color.h"
#include <string>
#include <vector>

class Board;

class Player {
private:
	unsigned char _id;
	std::string _name;
	Color _color;
	Board * _board = nullptr;
	
public:
	inline static std::vector<Player> players = std::vector<Player>();
	inline static void AddPlayer(const std::string & name, const Color & color) { players.push_back(Player(name, color)); }

	Player(const std::string &, const Color &);
	inline void SetBoard(Board* b) { _board = b; }
	void print(void) const;
};

#endif
