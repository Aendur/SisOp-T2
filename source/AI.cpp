#include "AI.h"
#include "board.h"
#include "utility.h"
#include "player.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <thread>

#define umap std::unordered_map

using std::string;
using std::vector;
using std::ifstream;

AI::AI(const string & path, const Player & p, const Board & board) : player(p) {
	this->width = board.width();
	this->height = board.height();
	this->LoadAI(path);
	this->_generator = std::mt19937(_seed);
	this->board_view = vector<bool>(width * height);
}

void AI::LoadAI(const std::string & path) {
	ifstream stream(path + ".ai");
	string line;
	int nline = 0;
	while( std::getline(stream, line) ) {
		ParseLine(line, ++nline);
	}
	stream.close();
}

void AI::ParseLine(const string & line, int nline) {
	if (line.length() > 2 && line[0] != '#') {
		auto v1 = Utility::split(line, "=");

		if (v1.size() != 2) { throw std::runtime_error("error: invalid arguments on line " + std::to_string(nline) + ":\n" + line); }
		ParseAttr(Utility::strip(v1[0]), v1[1]);
	}
}

struct arg_feats {
	unsigned int argc;
	string argdesc;
	void (AI::*argf)(const vector<string> &);
};

void AI::ParseAttr(const string & attr, const string & args) {
	static const umap<string, arg_feats> arg_act = {
		{ "SEED",  {1, "(seed)", &AI::SetSeed } },
		{ "DELAY", {1, "(ms)"  , &AI::SetDelay} },
	};

	vector<string> argv = Utility::split(args, ",");
	unsigned int argc_expected = arg_act.at(attr).argc;
	if (argv.size() != argc_expected) { throw std::runtime_error(attr + " expects " + std::to_string(argc_expected) + " arguments " + arg_act.at(attr).argdesc); }
	try {
		(this->*(arg_act.at(attr).argf))(argv);
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
	}
}

void AI::SetSeed(const vector<string> & args) {
	unsigned long long s = std::stoull(args[0]);
	if (s == 0) {
		this->_seed = std::random_device()();
	} else {
		this->_seed = s;
	}
}

void AI::SetDelay(const vector<string> & args) {
	this->delay = std::stoull(args[0]);
}

// void print_cores(int id, const std::pair<int,int> & chosen, int neibs, const std::set<std::pair<int,int>> & cores) {
// 	if (id == 1) {
// 		//std::ofstream logfile("player" + std::to_string(id) + ".log", std::ios_base::openmode::_S_app);
// 		std::cout << id << " chosen: (" << chosen.first << ',' << chosen.second << ") neibs: " << neibs << " |";
// 		for (const auto & core : cores) {
// 			std::cout << " (" << core.first << ',' << core.second << ')';
// 		}
// 		std::cout << std::endl;
// 		//logfile.close();
// 	}
// }

std::pair<int, int> AI::NextMove(void) {
	if (nmoves == 0) {
		int id = player.GetId();
		std::cout << id << " attempting first move..." << std::endl;
		std::uniform_int_distribution<int> dist_i(0, this->height - 1);
		std::uniform_int_distribution<int> dist_j(0, this->width - 1);
		return { dist_i(_generator), dist_j(_generator) };
	} else {
		if (partial_cores.size() > 0) {
			auto pair = GetNextExpansionCoords();
			return pair;
		} else {
			int id = player.GetId();
			std::cout << id << " is out of moves." << std::endl;
			
			has_moves = false;
			return {-1,-1};
		}
	}
}

const std::pair<int,int> AI::GetNextExpansionCoords(void) {
	long long total_weight = 0;
	for (const auto & [k,v] : partial_cores) { total_weight += v; }
	if (player.GetId() == 0) {
		std::cout << total_weight << std::endl;
	}
	std::uniform_int_distribution<long long> shuffle(0, total_weight - 1);
	long long pair_index = shuffle(_generator);
	auto pair = partial_cores.begin(); 
	pair_index -= pair->second;
	while(pair_index >= 0) {
		if (player.GetId() == 0) {
			std::cout << (pair->first.first) << ',' << (pair->first.second) << ',' << (pair->second) << ' ';
		}
		++pair;
		pair_index -= pair->second;
	}
	if (player.GetId() == 0) {
			std::cout << std::endl;
	}
	return pair->first;
}

const std::vector<std::pair<int,int>> AI::GetNeighbors8(const std::pair<int,int> & coords) const {
	int i = coords.first;
	int j = coords.second;
	std::vector<std::pair<int,int>> result;

	for (int i2 = i - 1; i2 <= i + 1; ++i2) {
		for (int j2 = j - 1; j2 <= j + 1; ++j2) {
			bool i_range_ok = (0 <= i2 && i2 < height);
			bool j_range_ok = (0 <= j2 && j2 < width);
			bool not_ij = !(i2 == i && j2 == j);
			bool is_free = (board_view[i2 * width + j2] == false);
			
			if (i_range_ok && j_range_ok && not_ij && is_free) {
				result.push_back({i2, j2});
			}
		}
	}
	return result;
}

const std::vector<std::pair<int,int>> AI::GetNeighbors4(const std::pair<int,int> & coords) const {
	int i = coords.first;
	int j = coords.second;
	std::vector<std::pair<int,int>> result;

	for (int i2 = i - 1; i2 <= i + 1; ++i2) {
		int j2 = j;
		bool i_range_ok = (0 <= i2 && i2 < height);
		bool not_ij = (i2 != i);
		bool is_free = (board_view[i2 * width + j2] == false);
		
		if (i_range_ok && not_ij && is_free) {
			result.push_back({i2, j2});
		}
	}
		
		
	for (int j2 = j - 1; j2 <= j + 1; ++j2) {
		int i2 = i;
		bool j_range_ok = (0 <= j2 && j2 < width);
		bool not_ij = (j2 != j);
		bool is_free = (board_view[i2 * width + j2] == false);
		
		if (j_range_ok && not_ij && is_free) {
			result.push_back({i2, j2});
		}
	}

	return result;
}

const std::vector<std::pair<int,int>> AI::GetNeighborsRK(const std::pair<int,int> & coords, int K) const {
	int i = coords.first;
	int j = coords.second;
	std::vector<std::pair<int,int>> result;

	for (int i2 = i - K; i2 <= i + K; ++i2) {
		for (int j2 = j - K; j2 <= j + K; ++j2) {
			bool i_range_ok = (0 <= i2 && i2 < height);
			bool j_range_ok = (0 <= j2 && j2 < width);
			bool not_ij = !(i2 == i && j2 == j);
			bool is_free = (board_view[i2 * width + j2] == false);
			bool radius = (abs(i - i2) + abs(j - j2)) <= K;
			
			if (i_range_ok && j_range_ok && not_ij && is_free && radius) {
				result.push_back({i2, j2});
			}
		}
	}
	return result;
}

void AI::ConfirmMove(int i, int j, bool marked) {
	board_view[i * width + j] = true;
	const std::pair<int,int> current = {i,j};
	if (marked) {
		if (partial_cores.find(current) != partial_cores.end()) {
			partial_cores.erase(current);
		}

		for (const auto & neighbor : GetNeighbors8(current)) {
			int ni = neighbor.first;
			int nj = neighbor.second;
			if (!board_view[ni * width + nj]) {
				partial_cores[neighbor] += 1;
			}
		}

		++nmoves;
	} else {
		if (partial_cores.find(current) != partial_cores.end()) {
			partial_cores.erase(current);
		}

		for (const auto & near : GetNeighborsRK(current, 4)) {
			if (partial_cores.find(near) != partial_cores.end()) {
				partial_cores[near] += width * height;
			}
		}
		for (const auto & near : GetNeighborsRK(current, 3)) {
			if (partial_cores.find(near) != partial_cores.end()) {
				partial_cores[near] += width * height * 10;
			}
		}
		for (const auto & near : GetNeighborsRK(current, 2)) {
			if (partial_cores.find(near) != partial_cores.end()) {
				partial_cores[near] += width * height * 100;
			}
		}
		for (const auto & near : GetNeighbors8(current)) {
			if (partial_cores.find(near) != partial_cores.end()) {
				partial_cores[near] += width * height * 1000;
			}
		}
	}
}

void AI::Print(void) const {
	std::cout << "SEED=" << this->_seed << '\n';
	std::cout << "DELAY=" << this->delay << '\n';
	std::cout << "nmoves=" << this->nmoves << std::endl;
}

void AI::Delay(void) const {
	auto t = std::chrono::duration<int, std::micro>(this->delay);
	std::this_thread::sleep_for(t);
}
