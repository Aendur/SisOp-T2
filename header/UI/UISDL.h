#ifndef UISDL_H
#define UISDL_H

#include "UI/UI.h"
#include "color.h"

#include <string>
#include <SDL2/SDL.h>

class SettingsUI;

class UISDL : public UI {
private:
	SDL_Window * window = nullptr;
	SDL_Renderer * renderer = nullptr;

	const std::string title;
	const SettingsUI & settings;
	const Board * board;

	int window_w;
	int window_h;

	bool initialized = false;
	bool stop = false;

	void InitializeSDL(void);
	void CreateWindow(void);
	void CreateRenderer(void);
	

	void HandleInput(void);
	void DrawBackground(void);
	void DrawBorder(void);
	void DrawGrid(void);
	void PaintCells(int, int, int, const Color &);
	void PaintRow(int);
	void DrawBoard(void);
public:
	UISDL(const std::string & title, const SettingsUI & settings);

	void Initialize(const Board*);
	void Dispose(void);
	void Refresh(int);
	inline void Clear(void) { }
	inline bool Stop(void) { return stop; }

};

#endif
