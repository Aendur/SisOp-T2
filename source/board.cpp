#include "board.h"
#include <cstdio>

#include "settings/settings.h"
#include "color.h"

size_t Board::GetSize(const Settings & settings) {
	return sizeof(Board)
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

	for (int i = 0; i < (int) settings.player_colors.size(); ++i) {
		//Color * colors = const_cast<Color*> (board->_color_list());
		board->_color_list(i)->R = settings.player_colors[i].R;
		board->_color_list(i)->G = settings.player_colors[i].G;
		board->_color_list(i)->B = settings.player_colors[i].B;
		board->_color_list(i)->A = 255;
	}

	for (int i = 0; i < board->_height; ++i) {
		for (int j = 0; j < board->_width; ++j) {
			*(board->_board(i, j)) = (cell_t) -1;
		}
	}
	printf("board initialized\n");
	return board;
}

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wpointer-arith"
Color * Board::_color_list(cell_t id) const {
	// void * addr = ((void*) this) + sizeof(Board);
	//return ((Color*) addr) + id;
	return (Color*)(this + 1) + id;
}

cell_t * Board::_board(int i, int j) const {
	int index = i * _width + j;
	//void * addr = ((void*) this) + sizeof(Board) + sizeof(Color) * this->_nplayers;
	//return ((cell_t*) addr) + index;
	return (cell_t*)((Color*)(this + 1) + this->_nplayers) + index;
}
// #pragma GCC diagnostic pop

void Board::Print(void) const {
	printf("width: %d height: %d\n", _width, _height);
	printf("nplayers: %d\n", _nplayers);
	printf("thisp @ %p\n", (void*) this);
	printf("colrs @ %p\n", (void*) _color_list(0));
	printf("board @ %p\n", (void*) _board(0,0));

	for (int i = 0; i < _nplayers; ++i) {
		const Color & c =  *_color_list(i);
		printf("Player %d (%d,%d,%d)\n", i, c.R, c.G, c.B);
	}
	for (int i = 0; i < _height; ++i) {
		for (int j = 0; j < _width; ++j) {
			printf("%4d", *(this->_board(i, j)));
		}
		printf("\n");
	}
}

void Board::Draw(void) const {
	char vsep = '#';
	printf("\033[1;1H\n ##########\n");
	printf(" %c Width: %d\n %c Height: %d\n", vsep, _width, vsep, _height);
	for (int i = 0; i < _nplayers; ++i) {
		const Color & c =  *_color_list(i);
		printf(" %c \033[48;2;%d;%d;%dm   \033[0m ", vsep, c.R, c.G, c.B);
		printf("Player %d (%d,%d,%d)\n", i, c.R, c.G, c.B);
	}
	printf(" ##########\n");
	for (int i = 0; i < _height; ++i) {
		printf(" %c", vsep);
		for (int j = 0; j < _width; ++j) {
			cell_t id = *_board(i, j);
			Color& c = *_color_list(id);
			printf("\033[48;2;%d;%d;%dm  \033[0m", c.R, c.G, c.B);
		}
		printf("%c\n", vsep);
	}
	printf(" ##########\n\n\033[0J");
}

bool Board::Mark(cell_t playerID, int i, int j) {
	bool marked = -1;
	if (0 <= i && i < _height && 0 <= j && j < _width) {
		if (*(this->_board(i, j)) == (cell_t) -1) {
			*(this->_board(i, j)) = playerID; // | (cell_t) 0x80;
			marked = true;
		} else {
			marked = false;
		}
	}
	return marked;
}

const std::map<cell_t, int> Board::CountScores(void) const {
	std::map<cell_t, int> scores;

	for (int i = 0; i < _height; ++i) {
		for (int j = 0; j < _width; ++j) {
			++scores[ (*_board(i,j)) ];
			//if ((*_board(i,j)) < 0) {
			//	++scores[ (*_board(i,j)) & (cell_t) 0x7F ];
			//} else {
			//	++scores[ (*_board(i,j)) ];
			//}
		}
	}

	return scores;
}

cell_t Board::Get(int i, int j) const {
	return *_board(i,j);
}

//cell_t Board::Flip(int i, int j) {
//	(*_board(i,j)) &= (cell_t) 0x7F;
//	return *_board(i,j);
//}
