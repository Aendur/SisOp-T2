#include "board.h"
#include <cstdio>

#include "settings/settings.h"
#include "color.h"

size_t Board::GetSize(const Settings & settings) {
	printf("SIZE %lu\n", sizeof(Board));
	return sizeof(Board)
		 + sizeof(int) * settings.num_players
		 + sizeof(Color) * settings.num_players
		 + sizeof(cell_t) * settings.grid_width * settings.grid_height
	;
}

Board* Board::Initialize(const Settings & settings, void * addr) {
	printf("initializing board at address %p... \n", addr);
	Board * board = (Board*) addr;
	board->_width = settings.grid_width;
	board->_height = settings.grid_height;
	board->_nplayers = settings.num_players;
	board->_last_id = -1;
	board->_offsets[OFFSET_SCORE] = sizeof(Board);
	board->_offsets[OFFSET_COLOR] = sizeof(Board) + sizeof(int) * board->_nplayers;
	board->_offsets[OFFSET_BOARD] = sizeof(Board) + sizeof(int) * board->_nplayers + sizeof(Color) * board->_nplayers;

	for (int i = 0; i < (int) settings.player_colors.size(); ++i) {
		*board->_scores(i) = 0;
		board->_colors(i)->R = settings.player_colors[i].R;
		board->_colors(i)->G = settings.player_colors[i].G;
		board->_colors(i)->B = settings.player_colors[i].B;
		board->_colors(i)->A = 255;
	}

	for (int i = 0; i < board->_height; ++i) {
		for (int j = 0; j < board->_width; ++j) {
			*(board->_board(i, j)) = (cell_t) -1;
		}
	}
	printf("board initialized\n");
	return board;
}

void Board::Print(void) const {
	printf("width: %d height: %d\n", _width, _height);
	printf("nplayers: %d\n", _nplayers);
	printf("thisp @ %p\n", (void*) this);
	printf("colrs @ %p\n", (void*) _colors(0));
	printf("board @ %p\n", (void*) _board(0,0));

	for (int i = 0; i < _nplayers; ++i) {
		const Color & c =  *_colors(i);
		int score = *_scores(i);
		printf("Player %d (%d,%d,%d) - %d\n", i, c.R, c.G, c.B, score);
	}
	for (int i = 0; i < _height; ++i) {
		for (int j = 0; j < _width; ++j) {
			printf("%4d", *(this->_board(i, j)));
		}
		printf("\n");
	}
}

bool Board::Mark(cell_t playerID, int i, int j) {
	bool marked = false;
	if (0 <= i && i < _height && 0 <= j && j < _width) {
		if (*(this->_board(i, j)) == (cell_t) -1) {
			*(this->_board(i, j)) = playerID; // | (cell_t) 0x80;
			*(this->_scores(playerID)) += 1;
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
		scores.insert({*_scores(p), p});
	}
	return scores;
}
