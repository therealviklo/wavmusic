#pragma once
#include "textmessagebox.h"

namespace TextMessageBoxUtils
{
	int max3(int a, int b, int c)
	{
		int maxOfFirstTwo = a > b ? a : b;
		return maxOfFirstTwo > c ? maxOfFirstTwo : c;
	}
}

template <typename CharType>
void TextMessageBox<CharType>::updateText(StringType text)
{
	this->text = std::move(text);
	
	int textWidth = 0;
	int rowCount = 1;
	int currRowWidth = 0;
	for (auto i = this->text.begin(); i != this->text.end(); i++)
	{
		if (*i == '\n')
		{
			if (currRowWidth > textWidth)
			{
				textWidth = currRowWidth;
			}
			currRowWidth = 0;
			rowCount++;
		}
		else
		{
			currRowWidth++;
		}
	}
	if (currRowWidth > textWidth)
	{
		textWidth = currRowWidth;
	}

	int buttonRowWidth = 0;
	for (auto i = options.begin(); i != options.end(); i++)
	{
		buttonRowWidth += i->size() + 2;
	}
	if (options.size() > 0) buttonRowWidth -= 2;
	
	width = TextMessageBoxUtils::max3(textWidth + 6, buttonRowWidth + 6, this->title.size() + 2);
	height = rowCount + 6;
}

template <typename CharType>
TextMessageBox<CharType>::TextMessageBox(StringType title, StringType text, std::vector<StringType> options, int defaultOption)
{
	this->title = std::move(title);
	this->text = std::move(text);
	this->options = std::move(options);
	selectedOption = defaultOption;

	int textWidth = 0;
	int rowCount = 1;
	int currRowWidth = 0;
	for (auto i = this->text.begin(); i != this->text.end(); i++)
	{
		if (*i == '\n')
		{
			if (currRowWidth > textWidth)
			{
				textWidth = currRowWidth;
			}
			currRowWidth = 0;
			rowCount++;
		}
		else
		{
			currRowWidth++;
		}
	}
	if (currRowWidth > textWidth)
	{
		textWidth = currRowWidth;
	}
	int buttonRowWidth = 0;
	for (auto i = this->options.begin(); i != this->options.end(); i++)
	{
		buttonRowWidth += i->size() + 2;
	}
	if (this->options.size() > 0) buttonRowWidth -= 2;
	width = TextMessageBoxUtils::max3(textWidth + 6, buttonRowWidth + 6, this->title.size() + 2);
	height = rowCount + 6;
}

template <typename CharType>
template <WORD colour, CharType LR, CharType UD, CharType DR, CharType LD, CharType LU, CharType UR>
void TextMessageBox<CharType>::draw(Textdisp& textdisp)
{
	int x = getX(textdisp);
	int y = getY(textdisp);

	int row = 0;
	int column = 0;

	//rad 1
	textdisp.set(x, y, DR, colour);
	// textdisp.set(x + 1, y, LR, colour);
	for (column = 1; column < title.size() + 1; column++)
	{
		textdisp.set(x + column, y, title.at(column - 1), colour);
	}
	for (; column < width - 1; column++)
	{
		textdisp.set(x + column, y, LR, colour);
	}
	textdisp.set(x + width - 1, y, LD, colour);

	row++;

	//rad 2
	textdisp.set(x, y + 1, UD, colour);
	for (int i = 1; i < width - 1; i++)
	{
		textdisp.set(x + i, y + 1, ' ', colour);
	}
	textdisp.set(x + width - 1, y + 1, UD, colour);

	row++;

	column = 3;

	//text
	textdisp.set(x, y + 2, UD, colour);
	textdisp.set(x + 1, y + 2, ' ', colour);
	textdisp.set(x + 2, y + 2, ' ', colour);
	for (auto i = text.begin(); i != text.end(); i++)
	{
		if (*i == '\n')
		{
			for (; column < width - 1; column++)
			{
				textdisp.set(x + column, y + row, ' ', colour);
			}
			textdisp.set(x + width - 1, y + row, UD, colour);
			textdisp.set(x, y + row + 1, UD, colour);
			textdisp.set(x + 1, y + row + 1, ' ', colour);
			textdisp.set(x + 2, y + row + 1, ' ', colour);
			column = 3;
			row++;
		}
		else
		{
			textdisp.set(x + column, y + row, *i, colour);
			column++;
		}
	}
	for (; column < width - 1; column++)
	{
		textdisp.set(x + column, y + row, ' ', colour);
	}
	textdisp.set(x + width - 1, y + row, UD, colour);

	row++;

	//tom rad
	textdisp.set(x, y + row, UD, colour);
	for (int i = 1; i < width - 1; i++)
	{
		textdisp.set(x + i, y + row, ' ', colour);
	}
	textdisp.set(x + width - 1, y + row, UD, colour);

	row++;
	column = 3;

	//knappar
	textdisp.set(x, y + row, UD, colour);
	textdisp.set(x + 1, y + row, ' ', colour);
	textdisp.set(x + 2, y + row, ' ', colour);
	for (int i = 0; i < options.size(); i++)
	{
		for (auto j = options[i].begin(); j != options[i].end(); j++)
		{
			textdisp.set(x + column, y + row, *j, i == selectedOption ? invertColour(colour) : colour);
			column++;
		}
		textdisp.set(x + column, y + row, ' ', colour);
		column++;
		textdisp.set(x + column, y + row, ' ', colour);
		column++;
	}
	for (; column < width - 1; column++)
	{
		textdisp.set(x + column, y + row, ' ', colour);
	}
	textdisp.set(x + width - 1, y + row, UD, colour);
	
	row++;

	//tom rad
	textdisp.set(x, y + row, UD, colour);
	for (int i = 1; i < width - 1; i++)
	{
		textdisp.set(x + i, y + row, ' ', colour);
	}
	textdisp.set(x + width - 1, y + row, UD, colour);
	
	row++;

	//sista raden
	textdisp.set(x, y + row, UR, colour);
	for (int i = 1; i < width - 1; i++)
	{
		textdisp.set(x + i, y + row, LR, colour);
	}
	textdisp.set(x + width - 1, y + row, LU, colour);
}

template <typename CharType>
void TextMessageBox<CharType>::updateCursorPos(bool leftPressed, bool rightPressed)
{
	if (leftPressed) selectedOption--;
	if (rightPressed) selectedOption++;
	selectedOption = (selectedOption + options.size()) % options.size();
}

template <>
TextInputBox<char>::TextInputBox(StringType title)
	: TextMessageBox<char>(
		std::move(title),
		"",
		{"OK"}
	)
{
}

template <>
TextInputBox<wchar_t>::TextInputBox(StringType title)
	: TextMessageBox<wchar_t>(
		std::move(title),
		L"",
		{L"OK"}
	)
{
}

template <typename CharType>
void TextInputBox<CharType>::addCharacter(CharType c)
{
	this->updateText(this->getText() + c);
}

template <typename CharType>
void TextInputBox<CharType>::removeCharacter()
{
	StringType str = this->getText();
	if (str.size() > 0)
	{
		this->updateText(str.substr(0, str.size() - 1));
	}
}

int promptTextMessageBoxA437(std::string title, std::string text, std::vector<std::string> options, Textdisp& textdisp, Timer& timer, int defaultOption)
{
	TextMessageBoxA textMessageBox(std::move(title), std::move(text), std::move(options));
	TextImage coveredArea(
		textdisp, 
		textMessageBox.getX(textdisp), 
		textMessageBox.getY(textdisp), 
		textMessageBox.getWidth(), 
		textMessageBox.getHeight()
	);

	while (true)
	{
		timer.start();

		textMessageBox.updateCursorPos(KB::KB.pressed('A') | KB::KB.pressed(VK_LEFT), KB::KB.pressed('D') | KB::KB.pressed(VK_RIGHT));

		if (KB::KB.pressed(VK_RETURN))
		{
			break;
		}

		textMessageBox.draw<ForegroundColour::black | BackgroundColour::white, 205, 186, 201, 187, 188, 200>(textdisp);
		textdisp.draw();
		timer.wait();
	}

	coveredArea.draw(textdisp, textMessageBox.getX(textdisp), textMessageBox.getY(textdisp));

	return textMessageBox.getSelectedOption();
}

void addCharacterAndRemoveBoxA(int c, std::unique_ptr<TextImage>& coveredArea, Textdisp& textdisp, TextInputBoxA& textInputBox)
{
	coveredArea->draw(textdisp, textInputBox.getX(textdisp), textInputBox.getY(textdisp));
	textInputBox.addCharacter(c);
	coveredArea = std::make_unique<TextImage>(
		textdisp, 
		textInputBox.getX(textdisp), 
		textInputBox.getY(textdisp), 
		textInputBox.getWidth(), 
		textInputBox.getHeight()
	);
}

std::string promptTextInputBoxA437(std::string title, Textdisp& textdisp, Timer& timer)
{
	TextInputBoxA textInputBox(std::move(title));
	std::unique_ptr<TextImage> coveredArea = std::make_unique<TextImage>(
		textdisp, 
		textInputBox.getX(textdisp), 
		textInputBox.getY(textdisp), 
		textInputBox.getWidth(), 
		textInputBox.getHeight()
	);

	while (true)
	{
		timer.start();

		if (KB::KB.typed(VK_BACK))
		{
			coveredArea->draw(textdisp, textInputBox.getX(textdisp), textInputBox.getY(textdisp));
			textInputBox.removeCharacter();
			coveredArea = std::make_unique<TextImage>(
				textdisp, 
				textInputBox.getX(textdisp), 
				textInputBox.getY(textdisp), 
				textInputBox.getWidth(), 
				textInputBox.getHeight()
			);
		}
		if (KB::KB.typed(VK_OEM_PLUS))
		{
			addCharacterAndRemoveBoxA('+', coveredArea, textdisp, textInputBox);
		}
		if (KB::KB.typed(VK_OEM_MINUS))
		{
			addCharacterAndRemoveBoxA('-', coveredArea, textdisp, textInputBox);
		}
		if (KB::KB.typed(VK_OEM_PERIOD))
		{
			addCharacterAndRemoveBoxA('.', coveredArea, textdisp, textInputBox);
		}
		if (KB::KB.typed(VK_OEM_COMMA))
		{
			addCharacterAndRemoveBoxA(',', coveredArea, textdisp, textInputBox);
		}
		if (KB::KB.typed(VK_SPACE))
		{
			addCharacterAndRemoveBoxA(' ', coveredArea, textdisp, textInputBox);
		}
		for (int c = '0'; c <= '9'; c++)
		{
			if (KB::KB.typed(c))
			{
				addCharacterAndRemoveBoxA(c, coveredArea, textdisp, textInputBox);
			}
		}
		if (KB::KB.down(VK_SHIFT) != KB::KB.capsToggled())
		{
			for (int c = 'A'; c <= 'Z'; c++)
			{
				if (KB::KB.typed(c))
				{
					addCharacterAndRemoveBoxA(c, coveredArea, textdisp, textInputBox);
				}
			}
		}
		else
		{
			for (int c = 'A'; c <= 'Z'; c++)
			{
				if (KB::KB.typed(c))
				{
					addCharacterAndRemoveBoxA(c - ('A' - 'a'), coveredArea, textdisp, textInputBox);
				}
			}
		}
		if (KB::KB.pressed(VK_RETURN))
		{
			break;
		}

		textInputBox.draw<ForegroundColour::black | BackgroundColour::white, 205, 186, 201, 187, 188, 200>(textdisp);
		textdisp.draw();
		timer.wait();
	}

	coveredArea->draw(textdisp, textInputBox.getX(textdisp), textInputBox.getY(textdisp));

	return textInputBox.getText();
}