#include "game.h"
#include "settings.h"
#include "player.h"
#include "board.h"
#include "UISDL.h"

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
	}
	for (Player * p : this->players) {
		p->print();
		p->Run();
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
	this->ui->Await(5000);
}

void Game::Run(void) {}