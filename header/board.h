#ifndef BOARD_H
#define BOARD_H

//#include <vector>

class Board {
private:
	int _width;
	int _height;
	//std::vector<std::vector<unsigned char>> _board;
	unsigned char * _board = nullptr;
public:
	Board(int, int);
	~Board(void);
	void print(void);
};

#endif
