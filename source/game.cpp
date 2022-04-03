#include "game.h"
#include "settings.h"
#include "player.h"
#include "board.h"
#include "UISDL.h"

#include <iostream>
#include <thread>
#include <chrono>

using std::string;

Game::~Game(void) {
	this->ui->Dispose();
	delete this->ui;
	
	for (Player * p : this->players) { delete p; }
	this->players.clear();

	delete this->board_main;
}

void Game::LoadSettings(const string & path) {
	this->settings.Load(path);
}

void Game::Initialize(void) {
	InitBoard();
	InitPlayers();
	InitUI();
}

void Game::InitPlayers(void) {
	for (auto & i : this->settings.players) {
		Player * p = new Player(i.second, *this);
		p->InitAI(i.first);
		p->Print();
		this->players.push_back(p);
	}
}

void Game::InitBoard(void) {
	this->board_main = new Board(settings.grid_width, settings.grid_height);
	this->board_main->print();
}

void Game::InitUI(void) {
	this->ui = new UISDL("test", this->settings);
	this->ui->Initialize();
	this->ui->DrawBoard();
	this->ui->Refresh(16);
}

void Game::RedrawUI(void) const {
	std::chrono::time_point t0 = std::chrono::steady_clock::now();
	this->ui->DrawBoard();
	for (int i = 0; i < board_main->height(); ++i) {
		for (int j = 0; j < board_main->width(); ++j) {
			signed char p = board_main->Get(i, j);
			if (p != -1) {
				ui->PaintCell(i, j, players[p]->GetColor());
			}
		}
	}
	std::chrono::time_point t1 = std::chrono::steady_clock::now();
	std::chrono::duration<long long, std::nano> dt = t1 - t0;
	std::chrono::duration<long long, std::nano> t(1000000000L / 24);
	int remaining = (t - dt).count()/1000000L;
	if (0 < remaining && remaining <= 40) {
		this->ui->Refresh(remaining);
	} else {
		this->ui->Refresh(1);
	}
}

void Game::Run(void) {
	std::cout << "init mainloop" << std::endl;

	std::vector<std::thread> threads;
	
	for (Player * p : players) {
		threads.emplace_back(&Player::Run, p);
	}

	while (!ui->Quit()) {
		RedrawUI();
		
		// flush board
		/*
		ui->HandleInput();
		auto pending = board_main->Flush();
		while (!pending.empty()) {
			auto & movement = pending.front();
			ui->PaintCell(movement.i, movement.j, players[movement.player]->GetColor());
			pending.pop();
		}
		ui->Refresh(16);
		*/
	}

	for (std::thread & t : threads) { t.join(); }

	for (Player * p : players) {
		p->Print();
	}

	for (const auto & [k,v] : board_main->CountScores()) {
		int id = k;
		std::cout << id << ": " << v << std::endl;
	}
}
