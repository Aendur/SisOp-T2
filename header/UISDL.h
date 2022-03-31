#ifndef UISDL_H
#define UISDL_H

#include "UI.h"
#include "color.h"

#include <string>
#include <SDL2/SDL.h>

class UISDL : public UI {
private:
	SDL_Window * window = nullptr;
	SDL_Renderer * renderer = nullptr;

	const std::string title;
	const Settings & settings;
	int width;
	int height;

	bool initialized = false;
	bool quit = false;

	void InitializeSDL(void);
	void CreateWindow(void);
	void CreateRenderer(void);
	
	void DrawBorder(void);
	void DrawGrid(void);

public:
	UISDL(const std::string & t, const Settings &);
	///~UI(void);

	void Initialize(void);
	void Dispose(void);

	void HandleInput(void);
	void DrawBoard(void);
	void PaintCell(int, int, const Color &);
	
	void Refresh(void);
	void Await(int);
	bool Quit(void) { return quit; }
};

#endif
