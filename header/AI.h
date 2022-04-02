#ifndef AI_H
#define AI_H

#include <vector>
#include <set>
#include <random>

class Board;

class AI {
private:
	unsigned long long _seed;
	std::mt19937  _generator;

	std::vector<bool> board_view;
	std::set<std::pair<int, int>> partial_cores;
	int width;
	int height;
	int nmoves = 0;

	bool has_moves = true;

	void LoadAI(const std::string &);
	void ParseLine(const std::string &, int);
	void ParseAttr(const std::string &, const std::string &);

	void SetSeed(const std::vector<std::string> &);

	std::vector<std::pair<int,int>> GetNeighbors(int, int);
public:
	AI(const std::string &, const Board &);
	//~AI(void);

	std::pair<int, int> NextMove(void);
	void ConfirmMove(int, int, bool);
	inline bool HasMoves(void) { return has_moves; }
	void Print(void) const;
};

#endif
