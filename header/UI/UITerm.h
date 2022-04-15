#ifndef UITERM_H
#define UITERM_H

#include "UI/UI.h"
#include "color.h"

#include <string>
#include <SDL2/SDL.h>

class SettingsUI;

class UITerm : public UI {
private:
	SDL_Window * window = nullptr;
	SDL_Renderer * renderer = nullptr;

	const std::string title;
	const SettingsUI & settings;
	const Board * board;

	int window_w;
	int window_h;

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
	void DrawBoard(void);
public:
	UITerm(const SettingsUI & settings);

	void Initialize(const Board*);
	void Dispose(void);
	void Refresh(int);
	void Clear(void);
	bool Stop(void) { return false; }
};

#endif


// printf("\n\n\n\n\n\n\n\n\n\n\n\n");
// printf("\n\n\n\n\n\n\n\n\n\n\n\n");
// printf("\n\n\n\n\n\n\n\n\n\n\n\n");
// printf("\n\n\n\n\n\n\n\n\n\n\n\n");
// printf("\n\n\n\n\n\n\n\n\n\n\n\n");
