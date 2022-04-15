#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H

#include "settings_base.h"

class SettingsUI : public SettingsBase {
private:
	void SetShowUI(const std::vector<std::string> &);
	void SetGridSize(const std::vector<std::string> &);
	void SetBorderSize(const std::vector<std::string> &);
	void SetLineColor(const std::vector<std::string> &);
	void SetBackgroundColor(const std::vector<std::string> &);
protected:
	const std::unordered_map<std::string, SettingsBase::Action> & GetActTable(void);
public:
	int GetWindowWidth(void) const;
	int GetWindowHeight(void) const;

	bool show_ui = false;
	int grid_width = 8;
	int grid_height = 8;
	int cell_size = 10;
	int border_size_outer = 15;
	int border_size_inner = 5;
	Color line_color = Color(0, 0, 0, 255);
	Color background_color = Color(127, 127, 127, 255);
};

#endif