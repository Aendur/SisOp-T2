#include "game.h"
#include "settings.h"
#include "player.h"
#include "board.h"
#include "UISDL.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <set>

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
		Player * p = new Player(i.second, this);
		p->InitAI(i.first);
		p->Print();
		this->players.push_back(p);
	}

	this->unfinished_players = (int) players.size();
}

void Game::InitBoard(void) {
	this->board_main = new Board(settings.grid_width, settings.grid_height);
	this->board_main->print();
	
	board_lock_col = std::vector<std::mutex>(settings.grid_width);
	board_lock_row = std::vector<std::mutex>(settings.grid_height);
	board_lock_cell = std::vector<std::mutex>(settings.grid_height * settings.grid_width);
}

void Game::InitUI(void) {
	this->ui = new UISDL("test", this->settings);
	this->ui->Initialize();
	this->ui->DrawBackground();
	this->ui->DrawBorder();
	this->ui->DrawGrid();
	this->ui->Refresh(16);
}

void Game::RedrawUI(void) const {
	std::chrono::time_point t0 = std::chrono::steady_clock::now();
	this->ui->DrawBackground();
	this->ui->DrawBorder();
	for (int i = 0; i < board_main->height(); ++i) {
		for (int j = 0; j < board_main->width(); ++j) {
			signed char p = board_main->Get(i, j);
			if (p != -1) {
				ui->PaintCell(i, j, players[p]->GetColor());
			}
		}
	}
	this->ui->DrawGrid();
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
		ui->HandleInput();
		RedrawUI();
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
		const Color & c = players[k]->GetColor();
		int id = players[k]->GetId();
		//std::cout << "\033[48;2;" << c.R << ';' << c.G << ';' << c.B << "m     \033[0m Player " << id << " - Score: " << -v << std::endl;
		printf("\033[48;2;%d;%d;%dm     \033[0m Player %d - Score: %d\n", c.R, c.G, c.B, id, -v);
	}
}

bool Game::MarkBoard(const Player & p, int i, int j) {
	//board_lock.lock();
	//board_lock_row[i].lock();
	//board_lock_col[j].lock();
	board_lock_cell[i * board_main->width() + j].lock();
	bool marked = this->board_main->Mark(p.GetId(), i, j);
	p.Delay();
	board_lock_cell[i * board_main->width() + j].unlock();
	//board_lock_col[j].unlock();
	//board_lock_row[i].unlock();
	//board_lock.unlock();

	return marked;
}

void Game::NotifyFinished(void) {
	notice_lock.lock();
	--unfinished_players;
	notice_lock.unlock();
}