#ifndef AI_H
#define AI_H

#include <vector>
#include <map>
#include <random>
#include <functional>

class Board;
class Player;

class AI {
private:
	unsigned long long _seed;
	std::mt19937  _generator;

	std::vector<bool> board_view;
	std::map<std::pair<int, int>, long long> partial_cores;
	int width;
	int height;
	int nmoves = 0;
	unsigned long long delay = 1;

	bool has_moves = true;
	const Player & player;

	void LoadAI(const std::string &);
	void ParseLine(const std::string &, int);
	void ParseAttr(const std::string &, const std::string &);

	void SetSeed(const std::vector<std::string> &);
	void SetDelay(const std::vector<std::string> &);

	const std::pair<int, int> GetNextExpansionCoords(void);
	const std::vector<std::pair<int,int>> GetNeighbors8(const std::pair<int,int> &) const;
	const std::vector<std::pair<int,int>> GetNeighbors4(const std::pair<int,int> &) const;
	const std::vector<std::pair<int,int>> GetNeighborsRK(const std::pair<int,int> &, int) const;
public:
	AI(const std::string &, const Player &, const Board &);
	//~AI(void);

	std::pair<int, int> NextMove(void);
	void ConfirmMove(int, int, bool);
	void Print(void) const;
	void Delay(void) const;
	
	inline bool HasMoves(void) { return has_moves; }
};

#endif
