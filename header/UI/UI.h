#ifndef UI_H
#define UI_H

//class Settings;
class Color;
class Board;

class UI {
public:
	UI(void) {}
	virtual ~UI(void) {};
	virtual void Initialize(Board*) = 0;
	virtual void Dispose(void) = 0;
	virtual void Refresh(int) = 0;
	virtual bool Online(void) = 0;

	// virtual void HandleInput(void) = 0;
	// virtual void DrawBackground(void) = 0;
	// virtual void DrawBorder(void) = 0;
	// virtual void DrawGrid(void) = 0;
	// virtual void PaintCell(int, int, const Color &) = 0;
	
};

#endif
