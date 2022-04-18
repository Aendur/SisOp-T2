#include "AI.h"

#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "sync.h"

using std::string;
using std::vector;

void AI::Initialize(const SettingsAI * settings, cell_t id, const Board & board) {
	this->player_id = id;
	this->width = board.GetWidth();
	this->height = board.GetHeight();
	this->settings = settings;
	this->board_view = vector<bool>(width * height);
	this->generator = std::mt19937(settings->seed);
}

std::pair<int, int> AI::NextMove(void) {
	int id = player_id;
	if (nmoves == 0) {
		//std::cout << id << " attempting first move..." << std::endl;
		int x0 = (2*settings->start_x - settings->var_x) * this->width / 200;
		int x1 = (2*settings->start_x + settings->var_x) * this->width / 200;
		int y0 = (2*settings->start_y - settings->var_y) * this->height / 200;
		int y1 = (2*settings->start_y + settings->var_y) * this->height / 200;
		if (x0 < 0) { x0 = 0; }
		if (x1 > this->width) { x1 = this->width; }
		if (x0 >= x1) { x0 = 0; x1 = this->width; }
		if (y0 < 0) { y0 = 0; }
		if (y1 > this->height) { y1 = this->height; }
		if (y0 >= y1)  { y0 = 0; y1 = this->height; }
		
		std::uniform_int_distribution<int> dist_i(y0, y1 - 1);
		std::uniform_int_distribution<int> dist_j(x0, x1 - 1);
		int i = dist_i(generator);
		int j = dist_j(generator);
		return { i, j };
	} else {
		//std::cout << id << " attempting " << nmoves << "th move..." << std::endl;
		if (partial_cores.size() > 0) {
			auto pair = GetNextExpansionCoords();
			return pair;
		} else {
			//std::cout << id << " is out of moves." << std::endl;
			has_moves = false;
			return {-1,-1};
		}
	}
}

const std::pair<int,int> AI::GetNextExpansionCoords(void) {
	long long total_weight = 0;
	for (const auto & [k,v] : partial_cores) { total_weight += v; }
	std::uniform_int_distribution<long long> shuffle(0, total_weight - 1);
	long long pair_index = shuffle(generator);
	auto pair = partial_cores.begin(); 
	pair_index -= pair->second;
	while(pair_index >= 0) {
		++pair;
		pair_index -= pair->second;
	}
	return pair->first;
}


int wrap_value(int v, int max, bool wrap, bool * range_ok) {
	if (0 <= v && v < max) {
		*range_ok = true;
		return v;
	} else {
		if (!wrap) {
			*range_ok = false;
			return -1;
		} else if (v < 0) {
			*range_ok = true;
			return max + (v % max);
		} else if (v >= max) {
			*range_ok = true;
			return v % max;
		}
	}
	throw std::logic_error("cannot wrap?");
}

const std::vector<std::pair<int,int>> AI::GetNeighbors8(const std::pair<int,int> & coords) const {
	int i = coords.first;
	int j = coords.second;
	std::vector<std::pair<int,int>> result;

	for (int i2 = i - 1; i2 <= i + 1; ++i2) {
		for (int j2 = j - 1; j2 <= j + 1; ++j2) {
			bool i_range_ok; // = (0 <= i2 && i2 < height);
			bool j_range_ok; // = (0 <= j2 && j2 < width);

			int i3 = wrap_value(i2, height, settings->wrap_y, &i_range_ok);
			int j3 = wrap_value(j2, width, settings->wrap_x, &j_range_ok);
			
			//bool not_ij = !(i2 == i && j2 == j);
			//bool is_free = (board_view[i2 * width + j2] == false);

			bool not_ij = !(i3 == i && j3 == j);
			bool is_free = (board_view[i3 * width + j3] == false);
			
			if (i_range_ok && j_range_ok && not_ij && is_free) {
				//result.push_back({i2, j2});
				result.push_back({i3, j3});
			}
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
			bool i_range_ok; // = (0 <= i2 && i2 < height);
			bool j_range_ok; // = (0 <= j2 && j2 < width);
			int i3 = wrap_value(i2, height, settings->wrap_y, &i_range_ok);
			int j3 = wrap_value(j2, width, settings->wrap_x, &j_range_ok);

			//bool not_ij = !(i2 == i && j2 == j);
			//bool is_free = (board_view[i2 * width + j2] == false);
			bool not_ij = !(i3 == i && j3 == j);
			bool is_free = (board_view[i3 * width + j3] == false);
			bool radius = (abs(i - i2) + abs(j - j2)) <= K;
			
			if (i_range_ok && j_range_ok && not_ij && is_free && radius) {
				//result.push_back({i2, j2});
				result.push_back({i3, j3});
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
	int id = this->player_id;
	std::cout << "id=" << id << '\n';
	std::cout << "SEED=" << this->settings->seed << '\n';
	std::cout << "DELAY=" << this->settings->delay << '\n';
	std::cout << "nmoves=" << this->nmoves << std::endl;
}

void AI::Delay(void) const {
	auto t = std::chrono::duration<unsigned long long, std::micro>(this->settings->delay);
	std::this_thread::sleep_for(t);
}

void AI::Delay(unsigned long long delay) const {
	auto t = std::chrono::duration<unsigned long long, std::micro>(delay);
	std::this_thread::sleep_for(t);
}
