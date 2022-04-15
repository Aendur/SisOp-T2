#include "game.h"
#include "board.h"
#include "UISDL.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <set>

using std::string;

void Game::InitUI(void) {
	this->ui = new UISDL("test", this->settings);
	this->ui->Initialize();
	this->ui->DrawBackground();
	this->ui->DrawBorder();
	this->ui->DrawGrid();
	this->ui->Refresh(16);
}

Game::~Game(void) {
	this->ui->Dispose();
	delete this->ui;
}

void Game::RedrawUI(void) const {
	std::chrono::time_point t0 = std::chrono::steady_clock::now();
	this->ui->DrawBackground();
	this->ui->DrawBorder();
	for (int i = 0; i < board_main->GetHeight(); ++i) {
		for (int j = 0; j < board_main->GetWidth(); ++j) {
			if (board_main->Get(i, j) < -1) {
				signed char p = board_main->Flip(i, j);
				//ui->PaintCell(i, j, players[p]->GetColor());
			}
		}
	}
	//this->ui->DrawGrid();
	std::chrono::time_point t1 = std::chrono::steady_clock::now();
	std::chrono::duration<long long, std::nano> dt = t1 - t0;
	std::chrono::duration<long long, std::nano> t(1000000000L / 24);
	int remaining = (t - dt).count()/1000000L;
	this->ui->Refresh(remaining);
}

void Game::UpdateUI(void) const {
	std::chrono::time_point t0 = std::chrono::steady_clock::now();
	for (int i = 0; i < board_main->GetHeight(); ++i) {
		for (int j = 0; j < board_main->GetWidth(); ++j) {
			if (board_main->Get(i, j) < -1) {
				signed char p = board_main->Flip(i, j);
				//ui->PaintCell(i, j, players[p]->GetColor());
			}
		}
	}
	std::chrono::time_point t1 = std::chrono::steady_clock::now();
	std::chrono::duration<long long, std::nano> dt = t1 - t0;
	std::chrono::duration<long long, std::nano> t(1000000000L / 24);
	int remaining = (t - dt).count()/1000000L;
	this->ui->Refresh(remaining);
}

void Game::Run(void) {
	std::cout << "init mainloop" << std::endl;

	std::vector<std::thread> threads;
	// for (Player * p : players) {
	// 	threads.emplace_back(&Player::Run, p);
	// }

	while (!ui->Quit()) {
		ui->HandleInput();
		//RedrawUI();
		UpdateUI();
		//ui->HandleInput();
		//ui->Refresh(25);

		if (unfinished_players == 0) {
			for (std::thread & t : threads) { t.join(); }
			this->PrintResults();
			unfinished_players = -1;
		}
	}
	
	printf("\n\n\n");
	for (std::thread & t : threads) { if (t.joinable()) { t.join(); } }
}

void Game::PrintResults(void) const {
	printf("\n\n\n");
	std::set<std::pair<int, signed char>> scores;
	for (const auto & [k,v] : board_main->CountScores()) { scores.insert({-v, k}); }
	for (const auto & [v,k] : scores) {
		//const Color & c = players[k]->GetColor();
		//int id = players[k]->GetId();
		//printf("\033[48;2;%d;%d;%dm     \033[0m Player %d - Score: %d\n", c.R, c.G, c.B, id, -v);
	}
}
