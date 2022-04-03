#include "settings.h"
#include "utility.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

using std::ifstream;
using std::string;
using std::vector;
#define umap std::unordered_map

void Settings::Load(const std::string & path) {
	ifstream stream(path);
	string line;
	int nline = 0;
	while( std::getline(stream, line) ) {
		ParseLine(line, ++nline);
	}
	stream.close();
}

void Settings::ParseLine(const string & line, int nline) {
	if (line.length() > 2 && line[0] != '#') {
		auto v1 = Utility::split(line, "=");

		if (v1.size() != 2) { throw std::runtime_error("error: invalid arguments on line " + std::to_string(nline) + ":\n" + line); }
		ParseAttr(Utility::strip(v1[0]), v1[1]);
	}
}

void Settings::ParseAttr(const string & attr, const string & args) {
	static const umap<string, std::pair<unsigned int, string>> arg_num = {
		{ "GRID_SIZE"        , {3, "(width,height,cell_size)" }},
		{ "LINE_COLOR"       , {3, "(R,G,B)"                  }},
		{ "BACKGROUND_COLOR" , {3, "(R,G,B)"                  }},
		{ "BORDER_SIZE"      , {2, "(outer,inner)"            }},
		{ "PLAYER"           , {4, "(name,R,G,B)"             }},
	};

	static const umap<string, void (Settings::*)(const vector<string> &)> arg_act = {
		{ "GRID_SIZE"        , &Settings::SetGridSize       },
		{ "LINE_COLOR"       , &Settings::SetLineColor      },
		{ "BACKGROUND_COLOR" , &Settings::SetBackgroundColor},
		{ "BORDER_SIZE"      , &Settings::SetBorderSize     },
		{ "PLAYER"           , &Settings::AddPlayer         },
	};

	vector<string> argv = Utility::split(args, ",");
	unsigned int argc_expected = arg_num.at(attr).first;
	if (argv.size() != argc_expected) { throw std::runtime_error(attr + " expects " + std::to_string(argc_expected) + " arguments " + arg_num.at(attr).second); }
	try {
		(this->*arg_act.at(attr))(argv);
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
	}
}

void Settings::SetGridSize(const vector<string> & argv) {
	this->grid_width = std::stoi(argv[0]);
	this->grid_height = std::stoi(argv[1]);
	this->cell_size = std::stoi(argv[2]);
}

void Settings::SetLineColor(const vector<string> & argv) {
	this->line_color.R = std::stoi(argv[0]);
	this->line_color.G = std::stoi(argv[1]);
	this->line_color.B = std::stoi(argv[2]);	
}

void Settings::SetBackgroundColor(const vector<string> & argv) {
	this->background_color.R = std::stoi(argv[0]);
	this->background_color.G = std::stoi(argv[1]);
	this->background_color.B = std::stoi(argv[2]);
}

void Settings::SetBorderSize(const vector<string> & argv) {
	this->border_size_outer = std::stoi(argv[0]);
	this->border_size_inner = std::stoi(argv[1]);
}

void Settings::AddPlayer(const vector<string> & argv) {
	Color c(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]), 255);
	players.push_back({argv[0], c});
}


int Settings::GetWindowWidth(void) const {
	return this->cell_size * this->grid_width + 2 * (this->border_size_outer + this->border_size_inner);
}

int Settings::GetWindowHeight(void) const {
	return this->cell_size * this->grid_height + 2 * (this->border_size_outer + this->border_size_inner);
}







