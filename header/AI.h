#ifndef AI_H
#define AI_H

#include "settings/settings_ai.h"
#include "board.h"
#include <vector>
#include <map>
#include <random>

class AI {
private:
	SettingsAI settings;
	std::mt19937 generator;

	std::vector<bool> board_view;
	std::map<std::pair<int, int>, long long> partial_cores;
	int width;
	int height;
	int nmoves = 0;

	bool initialized = false;
	bool has_moves = true;
	cell_t player_id;
	//const Player & player;

	const std::pair<int, int> GetNextExpansionCoords(void);
	const std::vector<std::pair<int,int>> GetNeighbors8(const std::pair<int,int> &) const;
	const std::vector<std::pair<int,int>> GetNeighbors4(const std::pair<int,int> &) const;
	const std::vector<std::pair<int,int>> GetNeighborsRK(const std::pair<int,int> &, int) const;
public:
	void Initialize(const char *, cell_t, const Board &);

	std::pair<int, int> NextMove(void);
	void ConfirmMove(int, int, bool);
	void Print(void) const;
	void Delay(void) const;
	void Delay(unsigned long long) const;
	inline unsigned long long GetDelay(void) const { return settings.delay; }
	
	inline bool HasMoves(void) { return has_moves; }
};

#endif
