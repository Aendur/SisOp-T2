#ifndef SETTINGS_BASE_H
#define SETTINGS_BASE_H

#include "color.h"

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class SettingsBase {
public:
	struct Action {
		unsigned int argc;
		std::string  argdesc;
		std::function<void(const std::vector<std::string> &)> action;
	};

	void Load(const char *);

protected:
	void ParseLine(const std::string &, int);
	void ParseAttr(const std::string &, const std::string &);

	virtual const std::unordered_map<std::string, SettingsBase::Action> & GetActTable(void) = 0;
};

#endif
