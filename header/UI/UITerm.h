#ifndef UITERM_H
#define UITERM_H

#include "UI/UI.h"
#include "board.h"

class SettingsUI;

class UITerm : public UI {
private:
	const SettingsUI & settings;
	const Board * board;

	char v_sep;
	char h_sep;
	char c_sep;
	char * border_h = nullptr;

	bool initialized = false;

	void DrawHLine(void);
	void DrawCells(int row);
	void DrawLBorder(void);
	void DrawRBorder(int row);
	void PrintPlayer(cell_t p);

public:
	UITerm(const SettingsUI & settings);

	void Initialize(const Board*);
	void Dispose(void);
	void Refresh(int);
	void Clear(void);
	inline bool Stop(void) { return false; }
};

#endif

