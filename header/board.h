#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <set>
#include <cstddef>
#include "color.h"

typedef signed short cell_t;
typedef std::set<std::pair<int, cell_t>, std::greater<std::pair<int, cell_t>>> result_t;

class Settings;

class Board {
private:
	int _width = 0;
	int _height = 0;
	int _nplayers = 0;
	cell_t _last_id = 0;

	std::vector<int> _scores;
	std::vector<Color> _colors;
	std::vector<cell_t> _board;

public:
	Board(const Settings &);
	
	void Print(void) const;

	bool Mark(cell_t, int, int);
	const result_t CountScores(void) const;

	inline int GetWidth(void) const { return _width; }
	inline int GetHeight(void) const { return _height; }
	inline int GetNPlayers(void) const { return _nplayers; }
	inline cell_t GetID(void) const { return _last_id; }
	inline cell_t AddID(void) { return ++_last_id; }
	
	inline int GetScore(cell_t id) const { return _scores[id]; }
	inline const Color & GetColor(cell_t id) const { return _colors[id]; }
	inline cell_t Get(int i, int j) const { return _board[i*_width+j]; }
};

#endif

