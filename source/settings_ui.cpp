#include "settings_ui.h"

//using std::ifstream;
using std::string;
using std::vector;
#define umap std::unordered_map


const umap<std::string, SettingsBase::Action> & SettingsUI::GetActTable(void) {
	static const umap<string, SettingsBase::Action> arg_num = {
		{ "SHOW_UI"          , {1, "(value)"                 , [this] (const vector<string> & argv) { this->SetShowUI(argv); }          }},
		{ "GRID_SIZE"        , {3, "(width,height,cell_size)", [this] (const vector<string> & argv) { this->SetGridSize(argv); }        }},
		{ "BORDER_SIZE"      , {2, "(outer,inner)"           , [this] (const vector<string> & argv) { this->SetBorderSize(argv); }      }},
		{ "LINE_COLOR"       , {4, "(R,G,B,A)"               , [this] (const vector<string> & argv) { this->SetLineColor(argv); }       }},
		{ "BACKGROUND_COLOR" , {3, "(R,G,B)"                 , [this] (const vector<string> & argv) { this->SetBackgroundColor(argv); } }},
	};
	return arg_num;
}

void SettingsUI::SetGridSize(const vector<string> & argv) {
	this->grid_width = std::stoi(argv[0]);
	this->grid_height = std::stoi(argv[1]);
	this->cell_size = std::stoi(argv[2]);
}

void SettingsUI::SetShowUI(const vector<string> & argv) {
	this->show_ui = (argv[0].length() == 1 && argv[0][0] == '1');
}

void SettingsUI::SetLineColor(const vector<string> & argv) {
	this->line_color.R = std::stoi(argv[0]);
	this->line_color.G = std::stoi(argv[1]);
	this->line_color.B = std::stoi(argv[2]);
	this->line_color.A = std::stoi(argv[3]);
}

void SettingsUI::SetBackgroundColor(const vector<string> & argv) {
	this->background_color.R = std::stoi(argv[0]);
	this->background_color.G = std::stoi(argv[1]);
	this->background_color.B = std::stoi(argv[2]);
}

void SettingsUI::SetBorderSize(const vector<string> & argv) {
	this->border_size_outer = std::stoi(argv[0]);
	this->border_size_inner = std::stoi(argv[1]);
}

int SettingsUI::GetWindowWidth(void) const {
	return this->cell_size * this->grid_width + 2 * (this->border_size_outer + this->border_size_inner);
}

int SettingsUI::GetWindowHeight(void) const {
	return this->cell_size * this->grid_height + 2 * (this->border_size_outer + this->border_size_inner);
}

