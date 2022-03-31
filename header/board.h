#ifndef BOARD_H
#define BOARD_H

#include <queue>
#include <mutex>

struct Movement {
	signed char player;
	int i;
	int j;
	Movement(void) : player(-1), i(0), j(0) {};
	Movement(signed char p, int i, int j) : player(p), i(i), j(j) {};
};

class Board {
private:
	int _width;
	int _height;
	signed char * _board = nullptr;
	std::mutex board_lock;
public:
	std::queue<Movement> pending_changes = std::queue<Movement>();

	Board(int, int);
	~Board(void);
	void print(void) const;
	void Mark(signed char, int, int);

	inline int width(void) { return _width; }
	inline int height(void) { return _height; }

};

#endif
