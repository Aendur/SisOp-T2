#include "game.h"
#include "settings.h"
#include "player.h"
#include "board.h"
#include "UISDL.h"

#include <iostream>
#include <thread>

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
	this->ui->Refresh();
}

void Game::Run(void) {
	std::cout << "init mainloop" << std::endl;

	std::vector<std::thread> threads;
	
	for (Player * p : players) {
		threads.emplace_back(&Player::Run, p);
	}

	while (!ui->Quit()) {
		ui->HandleInput();
		
		// flush board
		auto pending = board_main->Flush();
		while (!pending.empty()) {
			auto & movement = pending.front();
			//int id = movement.player;
			//std::cout << "i=" << movement.i << " j=" << movement.j << " id=" << id << std::endl;
			ui->PaintCell(movement.i, movement.j, players[movement.player]->GetColor());
			pending.pop();
		}
		
		
		ui->Refresh();
		ui->Await(16);
	}

	for (std::thread & t : threads) { t.join(); }

	board_main->print();
}
