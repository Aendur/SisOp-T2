#include "settings/settings_client.h"
#include <iostream>

using std::string;
using std::vector;
#define umap std::unordered_map

const umap<std::string, SettingsBase::Action> & SettingsClient::GetActTable(void) {
	static umap<string, SettingsBase::Action> arg_num = {
		{ "AI_FILE"      , {1, "(path)", [this] (const vector<string> & argv) { this->SetAIFile(argv);}  }},
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

void SettingsClient::SetAIFile(const vector<string> & argv) {
	this->ai_file = argv[0];
}

