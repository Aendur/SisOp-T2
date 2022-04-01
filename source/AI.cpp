#include "AI.h"
#include "player.h"

#include <iostream>

AI::AI(int width, int height, signed char p) : width(width), height(height), player(p) {
	this->_seed = std::random_device()();
	this->_generator = std::mt19937(_seed);
	this->board_view = std::vector<bool>(width * height);
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