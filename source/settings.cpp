#include "settings.h"
#include "utility.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

using std::ifstream;
using std::string;
using std::vector;
#define umap std::unordered_map

void Settings::LoadSettings(const std::string & path) {
	ifstream stream(path);
	string line;
	int nline = 0;
	while( std::getline(stream, line) ) {
		//std::cout << line << std::endl;
		ParseLine(line, ++nline);
	}
	stream.close();
}

void Settings::ParseLine(const string & line, int nline) {
	if (line.length() > 0 && line[0] != '#') {
		auto v1 = Utility::split(line, "=");

		if (v1.size() != 2) { throw std::runtime_error("error: invalid arguments on line " + std::to_string(nline) + ":\n" + line); }
		ParseAttr(Utility::strip(v1[0]), v1[1]);
	}
}

void Settings::ParseAttr(const string & attr, const string & args) {
	static const umap<string, std::pair<unsigned int, string>> arg_num = {
		{ "WINDOW_SIZE"     , {2, "(width,height)" }},
		{ "GRID_SIZE"       , {2, "(width,height)" }},
		{ "GRID_COLOR"      , {3, "(R,G,B)"        }},
		{ "BACKGROUND_COLOR", {3, "(R,G,B)"        }},
		{ "SQUARE_CELLS"    , {1, "(TRUE/FALSE)"   }},
		{ "FRAME_RATE"      , {1, "(fps)"          }},
		{ "PLAYER"          , {4, "(name,R,G,B)"   }},
	};

	static const umap<string, void (*)(const vector<string> &)> arg_act = {
		{ "WINDOW_SIZE"      , &Settings::SetWindowSize     },
		{ "GRID_SIZE"        , &Settings::SetGridSize       },
		{ "GRID_COLOR"       , &Settings::SetGridColor      },
		{ "BACKGROUND_COLOR" , &Settings::SetBackgroundColor},
		{ "SQUARE_CELLS"     , &Settings::SetSquareCells    },
		{ "FRAME_RATE"       , &Settings::SetFrameRate      },
		{ "PLAYER"           , &Settings::AddPlayer         },
	};

	vector<string> argv = Utility::split(args, ",");
	unsigned int argc_expected = arg_num.at(attr).first;
	if (argv.size() != argc_expected) { throw std::runtime_error(attr + " expects " + std::to_string(argc_expected) + " arguments " + arg_num.at(attr).second); }
	try {
		(*arg_act.at(attr))(argv);
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
	}
}

void Settings::SetWindowSize(const vector<string> & argv) {
	Settings::window_width = std::stoi(Utility::strip(argv[0]));
	Settings::window_height = std::stoi(Utility::strip(argv[1]));	
}

void Settings::SetGridSize(const vector<string> & argv) {
	Settings::grid_width = std::stoi(Utility::strip(argv[0]));
	Settings::grid_height = std::stoi(Utility::strip(argv[1]));
}

void Settings::SetGridColor(const vector<string> & argv) { (void) argv; throw std::logic_error("SetGridColor unimplemented"); }
void Settings::SetBackgroundColor(const vector<string> & argv) { (void) argv; throw std::logic_error("SetBackgroundColor unimplemented"); }
void Settings::SetSquareCells(const vector<string> & argv) { (void) argv; throw std::logic_error("SetSquareCells unimplemented"); }
void Settings::SetFrameRate(const vector<string> & argv) { (void) argv; throw std::logic_error("SetFrameRate unimplemented"); }
void Settings::AddPlayer(const vector<string> & argv) { (void) argv; throw std::logic_error("AddPlayer unimplemented"); }








