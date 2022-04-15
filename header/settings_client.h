#ifndef SETTINGS_CLIENT_H
#define SETTINGS_CLIENT_H

#include "settings_ui.h"

class SettingsClient : public SettingsUI {
private:
	void SetAIFile(const std::vector<std::string> &);
protected:
	const std::unordered_map<std::string, SettingsBase::Action> & GetActTable(void);
public:
	std::string ai_file;
};

#endif
