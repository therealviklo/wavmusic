#pragma once
#include "textutils.cpp"
#include "textdisp.cpp"
#include "textimage.cpp"
#include "sdltimer.cpp"
#include "keyboard.cpp"
#include <vector>
#include <string>
#include <memory>

struct MenuResult
{
	bool successful;
	std::string menu;
	std::string option;
};

class Menu
{
public:
	typedef std::vector<std::pair<std::string, std::vector<std::string>>> options_t;
private:
	options_t options;
	int menuSelectedOption;
	int listSelectedOption;
	bool isInMenu;

	Box getCoveredArea(int x, int y);
public:
	Menu(const options_t& options);

	bool inMenu() {return isInMenu;}

	void draw(int x, int y, Textdisp& textdisp);    
	MenuResult enterMenu(int x, int y, Textdisp& textdisp, Timer& timer);

	bool moveCursor(bool wPressed, bool aPressed, bool sPressed, bool dPressed);
};