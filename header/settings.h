#ifndef SETTINGS_H
#define SETTINGS_H

#include "color.h"

#include <string>
#include <vector>

class Settings {
private:
	void SetGridSize(const std::vector<std::string> &);
	void SetLineColor(const std::vector<std::string> &);
	void SetBackgroundColor(const std::vector<std::string> &);
	void SetBorderSize(const std::vector<std::string> &);
	void AddPlayer(const std::vector<std::string> &);

	void ParseLine(const std::string &, int);
	void ParseAttr(const std::string &, const std::string &);
public:
	int GetWindowWidth(void) const;
	int GetWindowHeight(void) const;

	int border_size_outer = 15;
	int border_size_inner = 5;
	int cell_size = 10;
	int grid_width = 8;
	int grid_height = 8;
	Color line_color = Color(0, 0, 0, 255);
	Color background_color = Color(127, 127, 127, 255);
	std::vector<std::pair<std::string, Color>> players;

	//Settings(const std::string &);
	void Load(const std::string &);
};

#endif
