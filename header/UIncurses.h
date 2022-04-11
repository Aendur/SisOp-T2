#ifndef UINCURSES_H
#define UINCURSES_H

#include "UI.h"
#include "color.h"

#include <ncurses.h>
#include <string>

class UIncurses : public UI {
private:
	WINDOW * window = nullptr;

	const std::string title;
	const Settings & settings;
	int width;
	int height;

	bool initialized = false;
	bool quit = false;

	void InitializeNcurses(void);
	void CreateWindow(void);

public:
	UIncurses(const std::string & t, const Settings &);
	///~UI(void);

	void Initialize(void);
	void Dispose(void);

	void HandleInput(void);
	void DrawBackground(void);
	void DrawBorder(void);
	void DrawGrid(void);
	void PaintCell(int, int, const Color &);

	void Refresh(int);
	bool Quit(void) { return quit; }
};

#endif
