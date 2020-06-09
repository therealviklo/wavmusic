#pragma once
#include <Windows.h>
#include <sstream>

#pragma comment(lib, "User32.lib")

namespace ForegroundColour
{
	enum ForegroundColour_t
	{
		black             = 0,
		darkblue          = FOREGROUND_BLUE,
		darkgreen         = FOREGROUND_GREEN,
		darkcyan          = FOREGROUND_GREEN | FOREGROUND_BLUE,
		darkred           = FOREGROUND_RED,
		darkmagenta       = FOREGROUND_RED | FOREGROUND_BLUE,
		darkyellow        = FOREGROUND_RED | FOREGROUND_GREEN,
		grey              = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		darkgrey          = FOREGROUND_INTENSITY,
		blue              = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		green             = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		cyan              = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		red               = FOREGROUND_INTENSITY | FOREGROUND_RED,
		magenta           = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		yellow            = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		white             = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};
}

namespace BackgroundColour
{
	enum BackgroundColour_t
	{
		black             = 0,
		darkblue          = BACKGROUND_BLUE,
		darkgreen         = BACKGROUND_GREEN,
		darkcyan          = BACKGROUND_GREEN | BACKGROUND_BLUE,
		darkred           = BACKGROUND_RED,
		darkmagenta       = BACKGROUND_RED | BACKGROUND_BLUE,
		darkyellow        = BACKGROUND_RED | BACKGROUND_GREEN,
		grey              = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
		darkgrey          = BACKGROUND_INTENSITY,
		blue              = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
		green             = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
		cyan              = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
		red               = BACKGROUND_INTENSITY | BACKGROUND_RED,
		magenta           = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
		yellow            = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
		white             = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	};
}

WORD invertColour(WORD colour);

class Textinp
{
private:
	struct KeyStruct
	{
		bool down; //om tangenten bokstavligen är nedtryckt
		bool pressed; //endast true en gång efter att den har tryckts ner
		bool typed; //true flera gånger efter att den har tryckts ner
	};

	KeyStruct keys[0xff]; //den sistan koden är 0xfe så 0xff stycken structar bör vara tillräckligt
	HANDLE handle;
	DWORD oldConsoleMode;

	void update();
public:
	Textinp();
	~Textinp();

	Textinp(const Textinp&) = delete;
	Textinp& operator=(const Textinp&) = delete;

	bool down(WORD key);
	bool pressed(WORD key);
	bool typed(WORD key);

	bool capsToggled() {return 1 & GetKeyState(VK_CAPITAL);}
	bool numLockToggled() {return 1 & GetKeyState(VK_NUMLOCK);}
	bool scrollLockToggled() {return 1 & GetKeyState(VK_SCROLL);}
};

class Textdisp
{
private:
	CHAR_INFO* screen;
	int width;
	int height;
	HANDLE screenBuffer;
	HANDLE stdScreenBuffer;
public:
	Textdisp();
	Textdisp(int width, int height);
	~Textdisp();

	Textdisp(const Textdisp&) = delete;
	Textdisp& operator=(const Textdisp&) = delete;

	int getWidth() { return width; };
	int getHeight() { return height; };

	void clear();
	void set(int x, int y, WCHAR c, WORD attributes);
	WCHAR getChar(int x, int y);
	WORD getAttributes(int x, int y);

	void draw();
};