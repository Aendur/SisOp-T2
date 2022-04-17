#include "board.h"
#include <cstdio>

#include "settings/settings.h"
#include "color.h"

Board::Board(const Settings & settings) {
	this->_width = settings.grid_width;
	this->_height = settings.grid_height;
	this->_nplayers = settings.num_players;
	this->_last_id = -1;

	this->_scores = std::vector<int> (this->_nplayers);

	for (const Color & color : settings.player_colors) {
		this->_colors.emplace_back(color.R, color.G, color.B, 255);
	}

	this->_board = std::vector<cell_t> (this->_width * this->_height);
	for (int i = 0; i < this->_height; ++i) {
		for (int j = 0; j < this->_width; ++j) {
			_board[i * _width + j] = -1;
		}
	}
}

void Board::Print(void) const {
	printf("width: %d height: %d\n", _width, _height);
	printf("nplayers: %d\n", _nplayers);
	for (int i = 0; i < _nplayers; ++i) {
		const Color & c =  GetColor(i);
		int score = GetScore(i);
		printf("Player %d (%d,%d,%d) - %d\n", i, c.R, c.G, c.B, score);
	}
	for (int i = 0; i < _height; ++i) {
		for (int j = 0; j < _width; ++j) {
			printf("%4d", Get(i, j));
		}
		printf("\n");
	}
}

bool Board::Mark(cell_t playerID, int i, int j) {
	bool marked = false;
	if (0 <= i && i < _height && 0 <= j && j < _width) {
		int index = i * _width + j;
		if (_board[index] == (cell_t) -1) {
			_board[index] = playerID;
			_scores[playerID] += 1;
			marked = true;
		} else {
			marked = false;
		}
	}
	return marked;
}

const result_t Board::CountScores(void) const {
	result_t scores;
	for (cell_t p = 0; p < _nplayers; ++p) {
		scores.insert({GetScore(p), p});
	}
	return scores;
}
