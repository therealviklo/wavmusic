#pragma once
#include <string>
#include <vector>
#include "timer.cpp"
#include "textimage.cpp"
#include "textdisp.cpp"

template <typename CharType>
class TextMessageBox
{
public:
	typedef std::basic_string<CharType> StringType;
private:
	StringType title;
	StringType text;
	int width;
	int height;
	std::vector<StringType> options;
	int selectedOption;
protected:
	void updateText(StringType text);
public:
	TextMessageBox(StringType title, StringType text, std::vector<StringType> options, int defaultOption = 0);

	int getX(Textdisp& textdisp) {return textdisp.getWidth() / 2 - width / 2;}
	int getY(Textdisp& textdisp) {return textdisp.getHeight() / 2 - height / 2;}
	int getWidth() {return width;}
	int getHeight() {return height;}
	StringType getText() {return text;}

	template <WORD colour, CharType LR, CharType UD, CharType DR, CharType LD, CharType LU, CharType UR>
	void draw(Textdisp& textdisp);

	void updateCursorPos(bool leftPressed, bool rightPressed);
	int getSelectedOption() {return selectedOption;}
};
typedef TextMessageBox<char> TextMessageBoxA;
typedef TextMessageBox<wchar_t> TextMessageBoxW;

template <typename CharType>
class TextInputBox : public TextMessageBox<CharType>
{
public:
	typedef std::basic_string<CharType> StringType;

	TextInputBox(StringType title);

	void addCharacter(CharType c);
	void removeCharacter();
};
typedef TextInputBox<char> TextInputBoxA;
typedef TextInputBox<wchar_t> TextInputBoxW;

//437 är code pagen
int promptTextMessageBoxA437(std::string title, std::string text, std::vector<std::string> options, Textdisp& textdisp, Timer& timer, int defaultOption = 0);
std::string promptTextInputBoxA437(std::string title, Textdisp& textdisp, Timer& timer);