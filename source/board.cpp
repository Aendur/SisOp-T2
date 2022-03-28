#include "board.h"
#include <cstdio>

Board::Board(int width, int height) : _width(width), _height(height) {
	//this->_board = std::vector<std::vector<unsigned char>>(height);
	//for (int j = 0; j < height; ++j) {
	//	this->_board[j] = std::vector<unsigned char>(width);
	//}
	
	this->_board = new unsigned char[width * height];

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			this->_board[i + j * width] = (unsigned char) 0;
		}
	}

}

Board::~Board(void) {
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
