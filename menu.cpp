#pragma once
#include "menu.h"

Box Menu::getCoveredArea(int x, int y)
{
	if (!isInMenu) return {0, 0, 0, 0};

	int xOffset = 0;
	for (int i = 0; i < options.size(); i++)
	{
		if (i == menuSelectedOption) break;
		xOffset += options[i].first.length() + 1;
	}

	int width = options.at(menuSelectedOption).first.size() + 1;
	for (int j = 0; j < options.at(menuSelectedOption).second.size(); j++)
	{
		if (options.at(menuSelectedOption).second[j].size() > width)
		{
			width = options.at(menuSelectedOption).second[j].size();
		}
	}

	return {x + xOffset, y + 1, x + xOffset + width, y + 1 + (int)options.at(menuSelectedOption).second.size()};
}

Menu::Menu(options_t options)
{
	this->options = std::move(options);
	menuSelectedOption = 0;
	listSelectedOption = 0;
	isInMenu = false;
}

void Menu::draw(int x, int y, Textdisp& textdisp)
{
	int offset = 0;
	for (int i = 0; i < options.size(); i++)
	{
		if (isInMenu && i == menuSelectedOption)
		{
			int yOffset = 1;
			int longestLength = options.at(menuSelectedOption).first.size() + 1;
			for (int j = 0; j < options[i].second.size(); j++)
			{
				if (options[i].second[j].size() > longestLength)
				{
					longestLength = options[i].second[j].size();
				}
			}
			for (int j = 0; j < options[i].second.size(); j++)
			{
				int xOffset = 0; //blanda inte ihop med offset
				for (auto c = options[i].second[j].begin(); c != options[i].second[j].end(); c++)
				{
					textdisp.set(x + offset + xOffset, y + yOffset, *c, j == listSelectedOption ? BackgroundColour::white | ForegroundColour::black : BackgroundColour::black | ForegroundColour::white);
					xOffset++;
				}
				while (xOffset < longestLength)
				{
					textdisp.set(x + offset + xOffset, y + yOffset, ' ', j == listSelectedOption ? BackgroundColour::white | ForegroundColour::black : BackgroundColour::black | ForegroundColour::white);
					xOffset++;
				}
				yOffset++;
			}
		}
		for (auto c = options[i].first.begin(); c != options[i].first.end(); c++)
		{
			textdisp.set(x + offset, y, *c, BackgroundColour::black | ForegroundColour::white);
			offset++;
		}
		textdisp.set(x + offset, y, L' ', BackgroundColour::black | ForegroundColour::white);
		offset++;
	}
}

MenuResult Menu::enterMenu(int x, int y, Textdisp& textdisp, Timer& timer)
{
	try
	{
		menuSelectedOption = 0;
		listSelectedOption = 0;
		isInMenu = true;
		Box coveredBox = getCoveredArea(x, y);
		std::unique_ptr<TextImage> coveredArea = std::make_unique<TextImage>(
			textdisp,
			coveredBox.left,
			coveredBox.top,
			coveredBox.right - coveredBox.left,
			coveredBox.bottom - coveredBox.top
		);

		while (true)
		{
			timer.start();

			if (KB::KB.pressed(VK_MENU))
			{
				isInMenu = false;
				return {false, "", ""};
			}
			if (KB::KB.pressed(VK_RETURN))
			{
				isInMenu = false;
				return {true, options.at(menuSelectedOption).first, options.at(menuSelectedOption).second.at(listSelectedOption)};
			}
			if (moveCursor(KB::KB.typed('W') | KB::KB.typed(VK_UP), KB::KB.typed('A') | KB::KB.typed(VK_LEFT), KB::KB.typed('S') | KB::KB.typed(VK_DOWN), KB::KB.typed('D') | KB::KB.typed(VK_RIGHT)))
			{
				if (coveredArea)
				{
					coveredArea->draw(textdisp, coveredBox.left, coveredBox.top);
				}
				coveredBox = getCoveredArea(x, y);
				coveredArea = std::make_unique<TextImage>(
					textdisp,
					coveredBox.left,
					coveredBox.top,
					coveredBox.right - coveredBox.left,
					coveredBox.bottom - coveredBox.top
				);
			}
			
			draw(x, y, textdisp);

			textdisp.draw();
			timer.wait();
		}
	}
	catch (...)
	{
		isInMenu = false;
		throw;
	}
	return {false, "", ""}; //borde ej köras
}

bool Menu::moveCursor(bool wPressed, bool aPressed, bool sPressed, bool dPressed)
{
	if (options.size() > 0)
	{
		if (aPressed)
		{
			menuSelectedOption = ((menuSelectedOption - 1) + options.size()) % options.size();
			listSelectedOption = 0;
		}
		if (dPressed)
		{
			menuSelectedOption = (menuSelectedOption + 1) % options.size();
			listSelectedOption = 0;
		}
		if (wPressed)
		{
			listSelectedOption = ((listSelectedOption - 1) + options.at(menuSelectedOption).second.size()) % options.at(menuSelectedOption).second.size();
		}
		if (sPressed)
		{
			listSelectedOption = (listSelectedOption + 1) % options.at(menuSelectedOption).second.size();
		}
	}
	return wPressed | aPressed | sPressed | dPressed;
}