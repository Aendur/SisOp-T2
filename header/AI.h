#ifndef AI_H
#define AI_H

#include "settings/settings_ai.h"
#include <vector>
#include <map>
#include <random>

class Board;
class Player;

class AI {
private:
	SettingsAI settings;
	std::mt19937 generator;

	std::vector<bool> board_view;
	std::map<std::pair<int, int>, long long> partial_cores;
	int width;
	int height;
	int nmoves = 0;

	bool has_moves = true;
	const Player & player;

	const std::pair<int, int> GetNextExpansionCoords(void);
	const std::vector<std::pair<int,int>> GetNeighbors8(const std::pair<int,int> &) const;
	const std::vector<std::pair<int,int>> GetNeighbors4(const std::pair<int,int> &) const;
	const std::vector<std::pair<int,int>> GetNeighborsRK(const std::pair<int,int> &, int) const;
public:
	AI(const char *, const Player &, const Board &);
	//~AI(void);

	std::pair<int, int> NextMove(void);
	void ConfirmMove(int, int, bool);
	void Print(void) const;
	void Delay(void) const;
	
	inline bool HasMoves(void) { return has_moves; }
};

#endif
