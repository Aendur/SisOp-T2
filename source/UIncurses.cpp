#include "UIncurses.h"
#include "settings.h"

#include <iostream>

UIncurses::UIncurses(const std::string & t, const Settings & settings) : title(t), settings(settings) {
	this->width  = settings.grid_width  + 2; // settings.GetWindowWidth();
	this->height = settings.grid_height + 2; // settings.GetWindowHeight();
}


void UIncurses::Initialize(void) {
	if (!this->initialized) {
		InitializeNcurses();
		CreateWindow();

		this->initialized = true;
		std::cout << "done." << std::endl;
	}
}

void UIncurses::Dispose(void) {
	if (this->initialized) {
		delwin(this->window);
		endwin();
		this->initialized = false;
	}
}

void UIncurses::InitializeNcurses(void) {
	std::cout << "initializing ncurses..." << std::endl;
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	intrflush(stdscr, FALSE);	
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
}

void UIncurses::CreateWindow(void) {
	//std::cout << "creating ncurses window..." << std::endl;
	//int begin_y = (LINES - height) / 2;
	//int begin_x = (COLS  - width) / 2;
	//this->window = newwin(height, width, begin_y, begin_x);
	//if (this->window == NULL) {
	//	throw std::runtime_error("failed to create window");
	//}
}

void UIncurses::HandleInput(void) {
	// SDL_Event event;
	// while (SDL_PollEvent(&event)) {
	// 	switch (event.type) {
	// 		case SDL_KEYUP:
	// 			if ((event.key.keysym.sym == SDLK_q) || (event.key.keysym.sym == SDLK_ESCAPE))
	// 				this->quit = true;
	// 			break;
	// 		case SDL_QUIT:
	// 			this->quit = true;
	// 			break;
	// 	}

	// }
}

void UIncurses::DrawBackground(void) {
	// SDL_SetRenderDrawColor(this->renderer, settings.background_color.R, settings.background_color.G, settings.background_color.B, settings.background_color.A);
	// SDL_RenderClear(this->renderer);
}

//  	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
// U+250x	─	━	│	┃	┄	┅	┆	┇	┈	┉	┊	┋	┌	┍	┎	┏
// U+251x	┐	┑	┒	┓	└	┕	┖	┗	┘	┙	┚	┛	├	┝	┞	┟
// U+252x	┠	┡	┢	┣	┤	┥	┦	┧	┨	┩	┪	┫	┬	┭	┮	┯
// U+253x	┰	┱	┲	┳	┴	┵	┶	┷	┸	┹	┺	┻	┼	┽	┾	┿
// U+254x	╀	╁	╂	╃	╄	╅	╆	╇	╈	╉	╊	╋	╌	╍	╎	╏
// U+255x	═	║	╒	╓	╔	╕	╖	╗	╘	╙	╚	╛	╜	╝	╞	╟
// U+256x	╠	╡	╢	╣	╤	╥	╦	╧	╨	╩	╪	╫	╬	╭	╮	╯
// U+257x	╰	╱	╲	╳	╴	╵	╶	╷	╸	╹	╺	╻	╼	╽	╾	

// 0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
// B				│	┤	╡	╢	╖	╕	╣	║	╗	╝	╜	╛	┐
// C	└	┴	┬	├	─	┼	╞	╟	╚	╔	╩	╦	╠	═	╬	╧
// D	╨	╤	╥	╙	╘	╒	╓	╫	╪	┘	┌					

void UIncurses::DrawBorder(void) {
	static const cchar_t c1 = {0, L"╔", 0};
	static const cchar_t c2 = {0, L"═", 0};
	static const cchar_t c3 = {0, L"╗", 0};
	static const cchar_t c4 = {0, L"║", 0};
	static const cchar_t c5 = {0, L"╚", 0};
	static const cchar_t c6 = {0, L"═", 0};
	static const cchar_t c7 = {0, L"╝", 0};
	//const wchar_t * corners =
	//	L"╔═╤╗"
	//	L"║█│║"
	//	L"╟─┼╢"
	//	L"╚═╧╝"
	//;
	mvadd_wch(0, 0, &c1);
	for (int j = 1; j < width - 1; ++j) { add_wch(&c2); }
	add_wch(&c3);

	for (int i = 1; i < height-1; ++i) {
		mvadd_wch(i,       0, &c4);
		mvadd_wch(i, width-1, &c4);
	}

	mvadd_wch(height-1, 0, &c5);
	for (int j = 1; j < width - 1; ++j) { add_wch(&c6); }
	add_wch(&c7);
}

void UIncurses::DrawGrid(void) {

}

#include <map>
int start_index = 100;
std::map<short, std::map<short, std::map<short, int>>> color_index;
void UIncurses::PaintCell(int i, int j, const Color & color) {
	short R = color.R * 4; R = R <= 1000 ? R : 1000;
	short G = color.G * 4; G = G <= 1000 ? G : 1000;
	short B = color.B * 4; B = B <= 1000 ? B : 1000;

	int index;
	if (color_index[R][G][B] == 0) {
		color_index[R][G][B] = start_index;
		init_color(start_index, R, G, B);
		init_pair(start_index, start_index, 0);
		index = start_index++;
	} else {
		index = color_index[R][G][B];
	}
	
	cchar_t c8 = {0, L"█", index};
	mvadd_wch(i+1, j+1, &c8);
	// const int size = settings.cell_size; // - 2;
	// const int x = settings.border_size_outer + settings.border_size_inner + settings.cell_size * j; // + 1;
	// const int y = settings.border_size_outer + settings.border_size_inner + settings.cell_size * i; // + 1;
	// const SDL_Rect cell = {x, y, size, size};

	// SDL_RenderFillRect(this->renderer, &cell);

	// if (settings.line_color.A > 0) {
	// 	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
	// 	SDL_SetRenderDrawColor(this->renderer, settings.line_color.R, settings.line_color.G, settings.line_color.B, settings.line_color.A);
	// 	SDL_RenderDrawRect(this->renderer, &cell);
	// }
}

void UIncurses::Refresh(int delay) {
	refresh();
}

//void UIncurses::Await(int t) {
//	SDL_Delay(t);
//}

