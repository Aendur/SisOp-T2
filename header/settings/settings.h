#ifndef SETTINGS_H
#define SETTINGS_H

#include "settings_ui.h"

class Settings : public SettingsUI {
private:
	void SetWaitForInput(const std::vector<std::string> &);
	void SetNumPlayers(const std::vector<std::string> &);
	void AddPlayerColor(const std::vector<std::string> &);
protected:
	const std::unordered_map<std::string, SettingsBase::Action> & GetActTable(void);
public:
	bool wait_for_input;
	int num_players;
	std::vector<Color> player_colors;
};

#endif
