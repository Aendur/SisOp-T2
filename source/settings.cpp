#include "settings.h"
#include <iostream>

using std::string;
using std::vector;
#define umap std::unordered_map

const umap<std::string, SettingsBase::Action> & Settings::GetActTable(void) {
	static umap<string, SettingsBase::Action> arg_num = {
		{ "NUM_PLAYERS"      , {1, "(nplayers)", [this] (const vector<string> & argv) { this->SetNumPlayers(argv);}  }},
		{ "PLAYER_COLOR"     , {3, "(R,G,B)"   , [this] (const vector<string> & argv) { this->AddPlayerColor(argv);} }},
	};
	static bool concat = false;
	if (!concat) {
		const auto & upper = this->SettingsUI::GetActTable();
		arg_num.insert(upper.begin(), upper.end());
		concat = true;
		std::cout << "concatenating act tables\n";
	}
	return arg_num;
}

void Settings::SetNumPlayers(const vector<string> & argv) {
	this->num_players = std::stoi(argv[0]);
}

void Settings::AddPlayerColor(const vector<string> & argv) {
	player_colors.emplace_back(
		std::stoi(argv[0]),
		std::stoi(argv[1]),
		std::stoi(argv[2]),
		255
	);
}

