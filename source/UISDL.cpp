#include "UISDL.h"
#include "settings.h"

#include <iostream>

UISDL::UISDL(const std::string & t, const Settings & settings) : title(t), settings(settings) {
	this->width = settings.GetWindowWidth();
	this->height = settings.GetWindowHeight();
}


void UISDL::Initialize(void) {
	if (!this->initialized) {
		InitializeSDL();
		CreateWindow();
		CreateRenderer();
		this->initialized = true;
		std::cout << "done." << std::endl;
	}
}

void UISDL::Dispose(void) {
	if (this->initialized) {
		std::cout << "deallocating renderer..." << std::endl;
		SDL_DestroyRenderer(this->renderer);
		this->renderer = nullptr;
		
		std::cout << "deallocating window..." << std::endl;
		SDL_DestroyWindow(this->window);
		this->window = nullptr;
		
		std::cout << "cleanup SDL..." << std::endl;
		SDL_Quit();
		
		std::cout << "done." << std::endl;

		this->initialized = false;
	}
}

void UISDL::InitializeSDL(void) {
	std::cout << "initializing SDL..." << std::endl;
	int status;
	status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	if (status != 0) {
		std::string error = SDL_GetError();
		throw std::runtime_error(error);
	}
}

void UISDL::CreateWindow(void) {
	std::cout << "creating SDL window..." << std::endl;
	int position = SDL_WINDOWPOS_UNDEFINED; // SDL_WINDOWPOS_CENTERED;
	unsigned int flags = SDL_WINDOW_OPENGL; // | SDL_WINDOW_BORDERLESS;
	this->window = SDL_CreateWindow(this->title.c_str(), position, position, this->width, this->height, flags);
	if (this->window == nullptr) {
		std::string error = SDL_GetError();
		throw std::runtime_error(error);
	}
}

void UISDL::CreateRenderer(void) {
	std::cout << "creating SDL renderer..." << std::endl;
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == nullptr) {
		std::string error = SDL_GetError();
		throw std::runtime_error(error);
	}
}

void UISDL::HandleInput(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYUP:
				if ((event.key.keysym.sym == SDLK_q) || (event.key.keysym.sym == SDLK_ESCAPE))
					this->quit = true;
				break;
			case SDL_QUIT:
				this->quit = true;
				break;
		}

	}
}

void UISDL::DrawBackground(void) {
	SDL_SetRenderDrawColor(this->renderer, settings.background_color.R, settings.background_color.G, settings.background_color.B, settings.background_color.A);
	SDL_RenderClear(this->renderer);
}

void UISDL::DrawBorder(void) {
	static const int outer_border_size = settings.border_size_outer;
	static const int total_border_size = settings.border_size_outer + settings.border_size_inner;
	static const int board_w = width  - 2 * total_border_size + 2;
	static const int board_h = height - 2 * total_border_size + 2;
	
	if (total_border_size > 0) {
		static const SDL_Rect border_outer[4] = {
			{ 0, 0, width, outer_border_size },
			{ 0, 0, outer_border_size, height },
			{ 0, height - outer_border_size, width, outer_border_size },
			{ width - outer_border_size, 0, outer_border_size, height },
		};
		
		static const SDL_Rect border_inner = { total_border_size - 1, total_border_size - 1, board_w, board_h };

		SDL_SetRenderDrawColor(this->renderer, settings.line_color.R, settings.line_color.G, settings.line_color.B, settings.line_color.A);
		if (outer_border_size > 0)
			SDL_RenderFillRects(this->renderer, border_outer, 4);
		if (settings.border_size_inner > 0)
			SDL_RenderDrawRect(this->renderer, &border_inner);
	}

}

void UISDL::DrawGrid(void) {
	static const int csize = settings.cell_size;
	static const int nrows = settings.grid_height;
	static const int ncols = settings.grid_width;
	static const int board_height = csize * nrows - 1;
	static const int board_width = csize * ncols - 1;
	
	if (settings.line_color.A > 0) {
		SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(this->renderer, settings.line_color.R, settings.line_color.G, settings.line_color.B, settings.line_color.A);
		
		int x0 = settings.border_size_outer + settings.border_size_inner;
		int y0 = x0;

		for(int i = 0; i < nrows; ++i) {
			const int y = y0 + i * csize;
			SDL_RenderDrawLine(this->renderer, x0, y            , x0 + board_width, y            );
			SDL_RenderDrawLine(this->renderer, x0, y + csize - 1, x0 + board_width, y + csize - 1);
		}

		for(int i = 0; i < ncols; ++i) {
			const int x = x0 + i * csize;
			SDL_RenderDrawLine(this->renderer, x            , y0, x            , y0 + board_height);
			SDL_RenderDrawLine(this->renderer, x + csize - 1, y0, x + csize - 1, y0 + board_height);
		}
	}

}

void UISDL::PaintCell(int i, int j, const Color & color) {
	SDL_SetRenderDrawColor(this->renderer, color.R, color.G, color.B, color.A);

	const int size = settings.cell_size; // - 2;
	const int x = settings.border_size_outer + settings.border_size_inner + settings.cell_size * j; // + 1;
	const int y = settings.border_size_outer + settings.border_size_inner + settings.cell_size * i; // + 1;
	const SDL_Rect cell = {x, y, size, size};

	SDL_RenderFillRect(this->renderer, &cell);

	if (settings.line_color.A > 0) {
		SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(this->renderer, settings.line_color.R, settings.line_color.G, settings.line_color.B, settings.line_color.A);
		SDL_RenderDrawRect(this->renderer, &cell);
	}
}

void UISDL::Refresh(int delay) {
	//this->HandleInput();
	SDL_RenderPresent(this->renderer);
	if (delay > 0) {
		SDL_Delay(delay);
	}
}

//void UISDL::Await(int t) {
//	SDL_Delay(t);
//}

