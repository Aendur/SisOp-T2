#ifndef SETTINGS_H
#define SETTINGS_H

#include "color.h"

#include <string>
#include <vector>
#include <unordered_map>
//#include <functional>

class Settings {
private:
	static void SetCellSize(const std::vector<std::string> &);
	static void SetGridSize(const std::vector<std::string> &);
	static void SetLineColor(const std::vector<std::string> &);
	static void SetBackgroundColor(const std::vector<std::string> &);
	static void SetBorderSize(const std::vector<std::string> &);
	static void SetFrameRate(const std::vector<std::string> &);
	static void AddPlayer(const std::vector<std::string> &);

	static void ParseLine(const std::string &, int);
	static void ParseAttr(const std::string &, const std::string &);
public:
	static int GetWindowWidth(void);
	static int GetWindowHeight(void);
	inline static int border_size1 = 15;
	inline static int border_size2 = 5;

	inline static int cell_size = 10;
	inline static int grid_width = 8;
	inline static int grid_height = 8;
	inline static Color line_color = Color(0, 0, 0, 255);
	inline static Color background_color = Color(127, 127, 127, 255);


	static void LoadSettings(const std::string &);
};

#endif
