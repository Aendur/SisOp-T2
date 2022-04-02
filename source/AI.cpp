#include "AI.h"
#include "board.h"
#include "utility.h"

#include <iostream>
#include <fstream>
#include <unordered_map>

#define umap std::unordered_map

using std::string;
using std::vector;
using std::ifstream;

AI::AI(const string & path, const Board & board) {
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
		{ "SEED", {1, "(seed)", &AI::SetSeed} },
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

std::pair<int, int> AI::NextMove(void) {
	if (nmoves == 0) {
		std::cout << "nm was 0: " << nmoves << std::endl;
		std::uniform_int_distribution<int> dist_x(0, this->width);
		std::uniform_int_distribution<int> dist_y(0, this->height);
		return { dist_y(_generator), dist_x(_generator) };
	} else {
		if (partial_cores.size() > 0) {
			std::cout << partial_cores.size() << std::endl;
			std::uniform_int_distribution<int> shuffle(0, partial_cores.size() - 1);
			int pair_index = shuffle(_generator);
			auto pair = partial_cores.begin(); 
			while(--pair_index >= 0) { ++pair; }

			auto neighbors = GetNeighbors(pair->first, pair->second);
			if (neighbors.size() == 0) {
				partial_cores.erase(pair);
				return {-1,-1};
			} else {
				std::uniform_int_distribution<int> dist(0, neighbors.size() - 1);
				return neighbors[dist(_generator)];
			}
		} else {
			has_moves = false;
			return {-1,-1};
		}
	}
}

std::vector<std::pair<int,int>> AI::GetNeighbors(int i, int j) {
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

void AI::ConfirmMove(int i, int j, bool marked) {
	board_view[i * width + j] = true;
	if (marked) {
		partial_cores.insert( {i,j} );
		++nmoves;
	}
}
