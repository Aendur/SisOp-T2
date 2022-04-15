#include "settings/settings_base.h"
#include "utility.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

using std::ifstream;
using std::string;
using std::vector;
#define umap std::unordered_map

void SettingsBase::Load(const char *path) {
	ifstream stream(path);
	string line;
	int nline = 0;
	while( std::getline(stream, line) ) {
		ParseLine(line, ++nline);
	}
	stream.close();
}

void SettingsBase::ParseLine(const string & line, int nline) {
	if (line.length() > 2 && line[0] != '#') {
		auto v1 = Utility::split(line, "=");

		if (v1.size() != 2) {
			throw std::runtime_error("error: invalid arguments on line " + std::to_string(nline) + ":\n" + line);
		}
		this->ParseAttr(Utility::strip(v1[0]), v1[1]);
	}
}

void SettingsBase::ParseAttr(const string & attr, const string & args) {
	const auto & arg_act = GetActTable();
	vector<string> argv = Utility::split(args, ",");
	unsigned int argc_expected = arg_act.at(attr).argc;
	if (argv.size() != argc_expected) { throw std::runtime_error(attr + " expects " + std::to_string(argc_expected) + " arguments " + arg_act.at(attr).argdesc); }
	try {
		arg_act.at(attr).action(argv);
		//(*arg_act.at(attr).action)(argv);
		//(this->*arg_act.at(attr))(argv);
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
	}
}

