#include "board.h"
#include <cstdio>

Board::Board(int width, int height) : _width(width), _height(height) {
	this->_board = new signed char[width * height];

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			this->_board[i + j * width] = (signed char) -1;
		}
	}

}

Board::~Board(void) {
	printf("clean up board...\n");
	delete[] this->_board;
	this->_board = nullptr;
}

void Board::print(void) const {
	for (int j = 0; j < _height; ++j) {
		for (int i = 0; i < _width; ++i) {
			printf("%4d", this->_board[i + j * _width]);
		}
		printf("\n");
	}
}


bool Board::Mark(signed char playerID, int i, int j) {
	bool marked = -1;
	if (0 <= i && i < _height && 0 <= j && j < _width) {
		int index = i * _width + j;
		if (this->_board[index] == (signed char) -1) {
			this->_board[index] = playerID | (signed char) 0x80;
			marked = true;
		} else {
			marked = false;
		}
	}
	return marked;
}

// std::queue<Movement> Board::Flush(void) {
// 	std::queue<Movement> flushed;
// 	board_lock.lock();

// 	while(!pending_changes.empty()) {
// 		flushed.push(pending_changes.front());
// 		pending_changes.pop();
// 	}

// 	board_lock.unlock();
// 	return flushed;
// }

const std::map<signed char, int> Board::CountScores(void) const {
	std::map<signed char, int> scores;

	for (int i = 0; i < (_width * _height); ++i) {
		if (_board[i] < 0) {
			++scores[_board[i] & (signed char) 0x7F];
		} else {
			++scores[_board[i]];
		}
	}

	return scores;
}

signed char Board::Get(int i, int j) const {
	return _board[i * _width + j];
}

signed char Board::Flip(int i, int j) {
	int index = i * _width + j;
	_board[index] &= (signed char) 0x7F;
	return _board[index];
}
