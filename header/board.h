#ifndef BOARD_H
#define BOARD_H

#include <queue>
#include <map>

typedef signed char cell_t;

class Board {
private:
	int _width = 0;
	int _height = 0;
	cell_t * _board = nullptr;
public:
	//Board(void);
	void Initialize(int width, int height, void* addr);

	void Print(void) const;
	bool Mark(cell_t, int, int);
	cell_t Get(int, int) const;
	cell_t Flip(int, int);
	const std::map<cell_t, int> CountScores(void) const;

	inline int GetWidth(void) const { return _width; }
	inline int GetHeight(void) const { return _height; }
};

#endif
