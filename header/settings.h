#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>
#include <unordered_map>
//#include <functional>

class Settings {
private:
	inline static int window_width = 800;
	inline static int window_height = 600;
	inline static int grid_width = 8;
	inline static int grid_height = 8;


	static void SetWindowSize(const std::vector<std::string> &);
	static void SetGridSize(const std::vector<std::string> &);
	static void SetGridColor(const std::vector<std::string> &);
	static void SetBackgroundColor(const std::vector<std::string> &);
	static void SetSquareCells(const std::vector<std::string> &);
	static void SetFrameRate(const std::vector<std::string> &);
	static void AddPlayer(const std::vector<std::string> &);

	static void ParseLine(const std::string &, int);
	static void ParseAttr(const std::string &, const std::string &);
public:
	static void LoadSettings(const std::string &);
};

#endif
