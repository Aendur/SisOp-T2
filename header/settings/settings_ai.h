#ifndef SETTINGS_AI_H
#define SETTINGS_AI_H

#include "settings_ui.h"

class SettingsAI : public SettingsUI {
private:
	void SetSeed(const std::vector<std::string> &);
	void SetDelay(const std::vector<std::string> &);
	void SetStart(const std::vector<std::string> &);
	void SetWrap(const std::vector<std::string> &);
protected:
	const std::unordered_map<std::string, SettingsBase::Action> & GetActTable(void);
public:
	unsigned long long seed = 0;
	unsigned long long delay = 1;
	int start_x = -1;
	int start_y = -1;
	int var_x = 0;
	int var_y = 0;
	bool wrap_x = false;
	bool wrap_y = false;
};

#endif
