#include "UI.h"
#include "settings.h"

#include <iostream>

void UI::InitializeSDL(void) {
	std::cout << "initializing SDL..." << std::endl;
	int status;
	status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	if (status != 0) {
		std::string error = SDL_GetError();
		throw std::runtime_error(error);
	}
}

void UI::CreateWindow(void) {
	std::cout << "creating SDL window..." << std::endl;
	this->window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, 0);
	if (this->window == nullptr) {
		std::string error = SDL_GetError();
		throw std::runtime_error(error);
	}
}

void UI::CreateRenderer(void) {
	std::cout << "creating SDL renderer..." << std::endl;
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == nullptr) {
		std::string error = SDL_GetError();
		throw std::runtime_error(error);
	}
}

UI::UI(const std::string & t, int w, int h) : title(t), width(w), height(h) {
	this->Initialize();
}

void UI::Initialize(void) {
	InitializeSDL();
	CreateWindow();
	CreateRenderer();
	std::cout << "done." << std::endl;
}

UI::~UI() {
	this->Dispose();
}

void UI::Dispose(void) {
	std::cout << "deallocating renderer..." << std::endl;
	SDL_DestroyRenderer(this->renderer);
	this->renderer = nullptr;
	
	std::cout << "deallocating window..." << std::endl;
	SDL_DestroyWindow(this->window);
	this->window = nullptr;
	
	std::cout << "cleanup SDL..." << std::endl;
	SDL_Quit();
	
	std::cout << "done." << std::endl;
}

void UI::Run(void) {
	while (!this->quit) {
		this->CalculateDeltaTime();

		this->HandleInput();
		this->DrawBoard();

		SDL_RenderPresent(this->renderer);
		SDL_Delay(32);
	}
}


void UI::HandleInput(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
		//case SDL_KEYDOWN:
		//	if (event.key.repeat) break;
		//	this->keyState[event.key.keysym.sym] = true;
		//	this->keyUpdate[event.key.keysym.sym] = this->updateCounter;
		//	break;
		//case SDL_KEYUP:
		//	this->keyState[event.key.keysym.sym] = false;
		//	this->keyUpdate[event.key.keysym.sym] = this->updateCounter;
		//	break;
		//case SDL_MOUSEBUTTONDOWN:
		//	this->mouseState[event.button.button] = true;
		//	this->mouseUpdate[event.button.button] = this->updateCounter;
		//	break;
		//case SDL_MOUSEBUTTONUP:
		//	this->mouseState[event.button.button] = false;
		//	this->mouseUpdate[event.button.button] = this->updateCounter;
		//	break;
		case SDL_QUIT:
			this->quit = true;
			break;
		default:
			//std::cout << "unhandled SDL event: " << event.type << std::endl;
			break;
		}
	}
}
////////////////////

void UI::CalculateDeltaTime(void) {
	int lastFrame = this->frameStart;
	this->frameStart = SDL_GetTicks();
	this->dt = (float) (this->frameStart - lastFrame) * 0.001;
	//std::cout << this->dt << std::endl;
}

void UI::DrawBoard(void) {
	Color bgc = Settings::background_color;
	SDL_SetRenderDrawColor(this->renderer, bgc.R, bgc.G, bgc.B, bgc.A);
	SDL_RenderClear(this->renderer);

	DrawBorder();
	DrawGrid();
}

void UI::DrawBorder(void) {
	static int outer_border_size = Settings::border_size1;
	static int total_border_size = Settings::border_size1 + Settings::border_size2;
	static int board_w = width  - 2 * total_border_size + 2;
	static int board_h = height - 2 * total_border_size + 2;
	static const SDL_Rect border_outer[4] = {
		{ 0, 0, width, outer_border_size },
		{ 0, 0, outer_border_size, height },
		{ 0, height - outer_border_size, width, outer_border_size },
		{ width - outer_border_size, 0, outer_border_size, height },
	};
	static const SDL_Rect border_inner = { total_border_size - 1, total_border_size - 1, board_w, board_h };

	Color lc = Settings::line_color;
	SDL_SetRenderDrawColor(this->renderer, lc.R, lc.G, lc.B, lc.A);
	SDL_RenderFillRects(this->renderer, border_outer, 4);
	SDL_RenderDrawRect(this->renderer, &border_inner);
}

void UI::DrawGrid(void) {
	static int csize = Settings::cell_size;
	static int nrows = Settings::grid_height;
	static int ncols = Settings::grid_width;
	static int board_height = csize * nrows - 1;
	static int board_width = csize * ncols - 1;
	static Color lc = Settings::line_color;
	
	SDL_SetRenderDrawColor(this->renderer, lc.R, lc.G, lc.B, lc.A);
	
	int x0 = Settings::border_size1 + Settings::border_size2;
	int y0 = x0;

	for(int i = 0; i < nrows; ++i) {
		int y = y0 + i * csize;
		SDL_RenderDrawLine(this->renderer, x0, y            , x0 + board_width, y            );
		SDL_RenderDrawLine(this->renderer, x0, y + csize - 1, x0 + board_width, y + csize - 1);
	}

	for(int i = 0; i < ncols; ++i) {
		int x = x0 + i * csize;
		SDL_RenderDrawLine(this->renderer, x            , y0, x            , y0 + board_height);
		SDL_RenderDrawLine(this->renderer, x + csize - 1, y0, x + csize - 1, y0 + board_height);
	}
}