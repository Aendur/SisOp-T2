#ifndef UI_H
#define UI_H

//class Settings;
class Color;
class Board;

class UI {
public:
	UI(void) {}
	virtual ~UI(void) {};
	virtual void Initialize(const Board*) = 0;
	virtual void Dispose(void) = 0;
	virtual void Refresh(int) = 0;
	virtual void Clear(void) = 0;
	virtual bool Stop(void) = 0;
};

#endif
