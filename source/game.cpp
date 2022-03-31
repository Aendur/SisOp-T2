#include "game.h"
#include "settings.h"
#include "player.h"
#include "board.h"
#include "UISDL.h"

#include <iostream>

using std::string;

Game::~Game(void) {
	this->ui->Dispose();
	delete this->ui;
	for (Player * p : this->players) { delete p; }
	delete this->board_main;
	this->players.clear();
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
		Player * p = new Player(i.first, i.second);
		p->SetBoard(board_main);
		p->SetGame(this);
		this->players.push_back(p);
		p->print();
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
	//this->ui->Await(5000);
}

#include <thread>
void Game::Run(void) {
	std::cout << "init mainloop" << std::endl;

	std::vector<std::thread> threads;
	
	for (Player * p : players) {
		//std::thread t(Player::Run, p);
		//threads.push_back(t);
		threads.emplace_back(&Player::Run, p);
	}

	while (!ui->Quit()) {
		ui->HandleInput();
		
		// flush board

		while (!board_main->pending_changes.empty()) {
			auto & movement = board_main->pending_changes.front();

			int id = movement.player;
			std::cout << "i=" << movement.i << " j=" << movement.j << " id=" << id << std::endl;
			
			ui->PaintCell(movement.i, movement.j, players[movement.player]->color());
			board_main->pending_changes.pop();
		}
		
		
		ui->Refresh();
		ui->Await(30);
	}
	
	for (std::thread & t : threads) { t.join(); }

}
