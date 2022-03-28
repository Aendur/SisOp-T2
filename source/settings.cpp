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
		{ "GRID_SIZE"        , {2, "(width,height)" }},
		{ "CELL_SIZE"        , {1, "(size)"         }},
		{ "LINE_COLOR"       , {3, "(R,G,B)"        }},
		{ "BACKGROUND_COLOR" , {3, "(R,G,B)"        }},
		{ "BORDER_SIZE"      , {2, "(outer,inner)"  }},
		{ "FRAME_RATE"       , {1, "(fps)"          }},
		{ "PLAYER"           , {4, "(name,R,G,B)"   }},
	};

	static const umap<string, void (*)(const vector<string> &)> arg_act = {
		{ "GRID_SIZE"        , &Settings::SetGridSize       },
		{ "CELL_SIZE"        , &Settings::SetCellSize       },
		{ "LINE_COLOR"       , &Settings::SetLineColor      },
		{ "BACKGROUND_COLOR" , &Settings::SetBackgroundColor},
		{ "BORDER_SIZE"      , &Settings::SetBorderSize     },
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

void Settings::SetCellSize(const vector<string> & argv) {
	Settings::cell_size = std::stoi(argv[0]);
}

void Settings::SetGridSize(const vector<string> & argv) {
	Settings::grid_width = std::stoi(argv[0]);
	Settings::grid_height = std::stoi(argv[1]);
}

void Settings::SetLineColor(const vector<string> & argv) {
	Settings::line_color.R = std::stoi(argv[0]);
	Settings::line_color.G = std::stoi(argv[1]);
	Settings::line_color.B = std::stoi(argv[2]);	
}

void Settings::SetBackgroundColor(const vector<string> & argv) {
	Settings::background_color.R = std::stoi(argv[0]);
	Settings::background_color.G = std::stoi(argv[1]);
	Settings::background_color.B = std::stoi(argv[2]);
}

void Settings::SetBorderSize(const vector<string> & argv) {
	Settings::border_size1 = std::stoi(argv[0]);
	Settings::border_size2 = std::stoi(argv[1]);
}

void Settings::SetFrameRate(const vector<string> & argv) { (void) argv; throw std::logic_error("SetFrameRate unimplemented"); }

void Settings::AddPlayer(const vector<string> & argv) {
	Color c(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]), 255);
	Player::AddPlayer(argv[0], c);
}


int Settings::GetWindowWidth(void) {
	return Settings::cell_size * Settings::grid_width + 2 * (Settings::border_size1 + Settings::border_size2);
}

int Settings::GetWindowHeight(void) {
	return Settings::cell_size * Settings::grid_height + 2 * (Settings::border_size1 + Settings::border_size2);
}







