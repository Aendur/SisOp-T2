#include "settings/settings_ai.h"

#include <random>
#include <iostream>

using std::string;
using std::vector;
#define umap std::unordered_map

const umap<std::string, SettingsBase::Action> & SettingsAI::GetActTable(void) {
	static umap<string, SettingsBase::Action> arg_num = {
		{ "SEED",  {1, "(seed)"          , [this] (const vector<string> & argv) { this->SetSeed(argv); }  }},
		{ "DELAY", {1, "(ms)"            , [this] (const vector<string> & argv) { this->SetDelay(argv); } }},
		{ "START", {4, "(X%%,Y%%,vX,vY)" , [this] (const vector<string> & argv) { this->SetStart(argv); } }},
		{ "WRAP",  {2, "(wrap_x,wrap_y)" , [this] (const vector<string> & argv) { this->SetWrap(argv); } }},
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

void SettingsAI::SetSeed(const vector<string> & args) {
	unsigned long long s = std::stoull(args[0]);
	if (s == 0) {
		this->seed = std::random_device()();
	} else {
		this->seed = s;
	}
}

void SettingsAI::SetDelay(const vector<string> & args) {
	this->delay = std::stoull(args[0]);
}

void SettingsAI::SetStart(const vector<string> & args) {
	this->start_x = std::stoi(args[0]);
	this->start_y = std::stoi(args[1]);
	this->var_x   = std::stoi(args[2]);
	this->var_y   = std::stoi(args[3]);
}

void SettingsAI::SetWrap(const vector<string> & args) {
	this->wrap_x = std::stoi(args[0]) == 1;
	this->wrap_y = std::stoi(args[1]) == 1;
}

