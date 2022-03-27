#include "UI.h"

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