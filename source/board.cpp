#include "board.h"
#include <cstdio>

// Board::Board(int width, int height, cell_t* board) : _width(width), _height(height), _board(board) {
// 	for (int j = 0; j < height; ++j) {
// 		for (int i = 0; i < width; ++i) {
// 			this->_board[i + j * width] = (cell_t) -1;
// 		}
// 	}
// }

void Board::Initialize(int width, int height, void * addr) {
	this->_width = width;
	this->_height = height;
	this->_board = (cell_t*) addr;

	if (_board[_width * _height / 2] == 0) {
		printf("initializing board... \n");
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				this->_board[i + j * width] = (cell_t) -1;
			}
		}
	}
	printf("board initialized\n");
}

void Board::Print(void) const {
	for (int j = 0; j < _height; ++j) {
		for (int i = 0; i < _width; ++i) {
			printf("%4d", this->_board[i + j * _width]);
		}
		printf("\n");
	}
}

bool Board::Mark(cell_t playerID, int i, int j) {
	bool marked = -1;
	if (0 <= i && i < _height && 0 <= j && j < _width) {
		int index = i * _width + j;
		if (this->_board[index] == (cell_t) -1) {
			this->_board[index] = playerID | (cell_t) 0x80;
			marked = true;
		} else {
			marked = false;
		}
	}
	return marked;
}

const std::map<cell_t, int> Board::CountScores(void) const {
	std::map<cell_t, int> scores;

	for (int i = 0; i < (_width * _height); ++i) {
		if (_board[i] < 0) {
			++scores[ _board[i] & (cell_t) 0x7F ];
		} else {
			++scores[ _board[i] ];
		}
	}

	return scores;
}

cell_t Board::Get(int i, int j) const {
	return _board[i * _width + j];
}

cell_t Board::Flip(int i, int j) {
	int index = i * _width + j;
	_board[index] &= (cell_t) 0x7F;
	return _board[index];
}
