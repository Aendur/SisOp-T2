#ifndef SETTINGS_AI_H
#define SETTINGS_AI_H

#include "settings_base.h"

class SettingsAI : public SettingsBase {
private:
	void SetSeed(const std::vector<std::string> &);
	void SetDelay(const std::vector<std::string> &);
protected:
	const std::unordered_map<std::string, SettingsBase::Action> & GetActTable(void);
public:
	unsigned long long seed = 0;
	unsigned long long delay = 1;
};

#endif
