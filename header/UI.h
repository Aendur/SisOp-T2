#ifndef UI_H
#define UI_H

#include <string>
#include <SDL2/SDL.h>

class UI {
private:
	SDL_Window * window = nullptr;
	SDL_Renderer * renderer = nullptr;

	std::string title;
	int width;
	int height;
	bool quit = false;

	void InitializeSDL(void);
	void CreateWindow(void);
	void CreateRenderer(void);

	void HandleInput(void);
	
	void Initialize(void);
	void Dispose(void);
	
	int frameStart = 0;
	float dt = -1.0f;
	void CalculateDeltaTime(void);

	void DrawBoard(void);
	void DrawBorder(void);
	void DrawGrid(void);

public:
	UI(const std::string & t, int w, int h);
	~UI(void);
	void Run(void);
};

#endif
