#include "UI/UITerm.h"
#include "settings/settings_ui.h"

#include <cstring>

UITerm::UITerm(const SettingsUI & settings) : settings(settings) {}

void UITerm::Initialize(const Board* b) {
	this->board = b;
	this->v_sep = '|';
	this->h_sep = '-';
	this->c_sep = '+';
	
	// const int offset = 1;
	// const int border = 2;
	// const int newlin = 1;
	// const int nullch = 1;
	const int extra_size = 5;
	
	int w = b->GetWidth() * 2 + extra_size;
	this->border_h = new char[w];
	memset(this->border_h, h_sep, w);
	this->border_h[0] = ' ';
	this->border_h[1] = c_sep;
	this->border_h[w-3] = c_sep;
	this->border_h[w-2] = '\n';
	this->border_h[w-1] = 0;
}

void UITerm::Dispose(void) {
	delete this->border_h;
	this->border_h = nullptr;
}

void UITerm::Clear(void) {
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
}

#include <thread>
#include <chrono>

void UITerm::Refresh(int delay) {
	printf("\033[1;1H\n");
	DrawHLine();
	for (int i = 0; i < board->GetHeight(); ++i) {
		DrawLBorder();
		DrawCells(i);
		DrawRBorder(i);
	}
	DrawHLine();

	if (delay > 0) {
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(delay));
	}
}

void UITerm::DrawCells(int row) {
	for (int col = 0; col < board->GetWidth(); ++col) {
		cell_t p = board->Get(row, col);
		const Color& c = (p < 0) ? settings.background_color : board->GetColor(p);
		printf("\033[48;2;%d;%d;%dm  \033[0m", c.R, c.G, c.B);
	}
}

void UITerm::DrawLBorder(void) {
	printf(" %c", v_sep);
}

void UITerm::DrawRBorder(int row) {
	printf("%c ", v_sep);
	
	//PrintPlayer(row - 1);
	switch (row) {
		case 0: printf("Scores\n"); break;
		default: PrintPlayer(row - 2); break;
		//case 0: printf("Width:  %d\n", board->GetWidth()); break;
		//case 1: printf("Height: %d\n", board->GetHeight()); break;
		//case 2: printf("Scores:\n"); break;
		//default: PrintPlayer(row - 3); break;
	}
}

void UITerm::PrintPlayer(cell_t p) {
	if (0 <= p && p < board->GetNPlayers()) {
		const Color & c =  board->GetColor(p);
		int score = board->GetScore(p);
		printf(" \033[48;2;%d;%d;%dm   \033[0m P%d: %d\n", c.R, c.G, c.B, p, score);
		
	} else {
		printf("\n");
	}
}

void UITerm::DrawHLine(void) {
	printf(this->border_h);
}

