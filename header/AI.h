#ifndef AI_H
#define AI_H

#include <vector>
#include <set>
#include <random>

class AI {
private:
	unsigned long long _seed;
	std::mt19937  _generator;

	std::vector<bool> board_view;
	std::set<std::pair<int, int>> partial_cores;
	int width;
	int height;
	int nmoves = 0;
	signed char player = -1;

	bool has_moves = true;

	std::vector<std::pair<int,int>> GetNeighbors(int, int);
public:
	AI(int width, int height, signed char p);
	//~AI(void);

	std::pair<int, int> NextMove(void);
	void ConfirmMove(int, int, bool);
	inline bool HasMoves(void) { return has_moves; }
};

#endif
