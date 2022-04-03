#ifndef UI_H
#define UI_H

class Settings;
class Color;
class UI {
//protected:
//	const Settings & settings;
public:
	//UI(const Settings & s) : settings(s) {}
	UI(void) {}
	virtual ~UI(void) {};

	virtual void Initialize(void) = 0;
	virtual void Dispose(void) = 0;

	virtual void HandleInput(void) = 0;
	virtual void DrawBoard(void) = 0;
	virtual void PaintCell(int, int, const Color &) = 0;
	
	virtual void Refresh(int) = 0;
	virtual void Await(int) = 0;
	virtual bool Quit(void) = 0;
};

#endif
