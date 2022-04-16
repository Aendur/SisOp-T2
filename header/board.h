#ifndef BOARD_H
#define BOARD_H

#include <queue>
#include <map>
#include <cstddef>
#include "color.h"

//typedef signed char cell_t;
typedef signed short cell_t;
class Settings;

#define OFFSET_SCORE 0
#define OFFSET_COLOR 1
#define OFFSET_BOARD 2

class Board {
private:
	int _width = 0;
	int _height = 0;
	int _nplayers = 0;
	int _offsets[3];
	cell_t _last_id = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
	inline int* _scores(cell_t id) const { return (int*)((void*)this + _offsets[OFFSET_SCORE]) + id; }
	inline Color* _colors(cell_t id) const { return (Color*)((void*)this + _offsets[OFFSET_COLOR]) + id; }
	inline cell_t* _board(int i, int j) const { return (cell_t*)((void*)this + _offsets[OFFSET_BOARD]) + (i * _width + j); }
#pragma GCC diagnostic pop
public:
	void Print(void) const;

	bool Mark(cell_t, int, int);
	const std::map<cell_t, int> CountScores(void) const;

	inline int GetWidth(void) const { return _width; }
	inline int GetHeight(void) const { return _height; }
	inline int GetNPlayers(void) const { return _nplayers; }
	inline cell_t GetID(void) const { return _last_id; }
	inline cell_t AddID(void) { return ++_last_id; }
	
	inline int GetScore(cell_t id) const { return *_scores(id); }
	inline const Color & GetColor(cell_t id) const { return *_colors(id); }
	inline cell_t Get(int i, int j) const { return *_board(i,j); }

	// board factory
	static Board* Initialize(const Settings &, void* addr);
	static size_t GetSize(const Settings &);
};

#endif

