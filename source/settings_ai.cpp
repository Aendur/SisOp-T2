#include "settings_ai.h"

using std::string;
using std::vector;
#define umap std::unordered_map

#include <random>

const umap<std::string, SettingsBase::Action> & SettingsAI::GetActTable(void) {
	static const umap<string, SettingsBase::Action> arg_num = {
		{ "SEED",  {1, "(seed)", [this] (const vector<string> & argv) { this->SetSeed(argv); }  }},
		{ "DELAY", {1, "(ms)"  , [this] (const vector<string> & argv) { this->SetDelay(argv); } }},
	};
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

