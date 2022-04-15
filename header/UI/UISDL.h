#ifndef UISDL_H
#define UISDL_H

#include "UI/UI.h"
#include "color.h"

#include <string>
#include <SDL2/SDL.h>

class Settings;

class UISDL : public UI {
private:
	SDL_Window * window = nullptr;
	SDL_Renderer * renderer = nullptr;

	const std::string title;
	const Settings & settings;
	int width;
	int height;

	bool initialized = false;
	bool online = true;

	void InitializeSDL(void);
	void CreateWindow(void);
	void CreateRenderer(void);
	

	void HandleInput(void);
	void DrawBackground(void);
	void DrawBorder(void);
	void DrawGrid(void);
	void PaintCell(int, int, const Color &);
public:
	UISDL(const std::string & t, const Settings &);

	void Initialize(Board*);
	void Dispose(void);
	void Refresh(int);
	bool Online(void) { return online; }
};

#endif
