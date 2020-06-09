#pragma once
#include "textdisp.h"

WORD invertColour(WORD colour)
{
	return colour ^ FOREGROUND_RED ^ FOREGROUND_GREEN ^ FOREGROUND_BLUE ^ FOREGROUND_INTENSITY ^ BACKGROUND_RED ^ BACKGROUND_GREEN ^ BACKGROUND_BLUE ^ BACKGROUND_INTENSITY;
}

void Textinp::update()
{
	DWORD numberOfEvents;
	if (!GetNumberOfConsoleInputEvents(handle, &numberOfEvents))
		throw std::runtime_error("Failed to get number of console input events");
	if (numberOfEvents > 0)
	{
		INPUT_RECORD events[numberOfEvents];
		DWORD numberOfEventsRead;
		if (!ReadConsoleInputW(handle, events, numberOfEvents, &numberOfEventsRead))
			throw std::runtime_error("Failed to read console input events");
		for (int i = 0; i < numberOfEventsRead; i++)
		{
			switch (events[i].EventType)
			{
			case KEY_EVENT:
				{
					/*
					 * Av någon anledning verkar Windows Terminal skicka ett meddelande om att tangenten har släppts
					 * upp omedelbart efter att tangenten har tryckts ner, även om man fortsätter att hålla ner
					 * tangenten. På grund av detta har jag gjort det så att KeyStruct::pressed och KeyStruct::typed
					 * fortsätter att vara true tills man läser dem med Textinp::pressed respektive Textinp::typed, även
					 * om man slutar hålla ner tangenten. Att Textinp::down är oanvändbar på Windows Terminal får vi
					 * leva med. (Skriver man "vi" när det säkert bara är en framtida version av en själv som kommer
					 * att läsa kommentaren?)
					 * 
					 * För övrigt verkar Window Terminal endast skicka event för skift o.s.v. när en annan tangent
					 * trycks ner.
					*/
					if (!keys[events[i].Event.KeyEvent.wVirtualKeyCode].down && events[i].Event.KeyEvent.bKeyDown)
					{
						keys[events[i].Event.KeyEvent.wVirtualKeyCode].pressed = true;
					}
					keys[events[i].Event.KeyEvent.wVirtualKeyCode].down = events[i].Event.KeyEvent.bKeyDown;
					keys[events[i].Event.KeyEvent.wVirtualKeyCode].typed = //raden var för lång
						keys[events[i].Event.KeyEvent.wVirtualKeyCode].typed || events[i].Event.KeyEvent.bKeyDown;
				}
				break;
			default:
				break;
			}
		}
	}
}

Textinp::Textinp()
	: keys({})
{
	handle = GetStdHandle(STD_INPUT_HANDLE);
	if (handle == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Failed to get standard input handle");
	if (!GetConsoleMode(handle, &oldConsoleMode))
		throw std::runtime_error("Failed to get console mode");
	if (!SetConsoleMode(handle, ENABLE_EXTENDED_FLAGS | ENABLE_INSERT_MODE))
		throw std::runtime_error("Failed to set console mode");
}

Textinp::~Textinp()
{
	try
	{
		update();
	}
	catch (...) {}
	SetConsoleMode(handle, oldConsoleMode);
}

bool Textinp::down(WORD key)
{
	update();
	return keys[key].down;
}

bool Textinp::pressed(WORD key)
{
	update();
	if (keys[key].pressed)
	{
		keys[key].pressed = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool Textinp::typed(WORD key)
{
	update();
	if (keys[key].typed)
	{
		keys[key].typed = false;
		return true;
	}
	else
	{
		return false;
	}
}

Textdisp::Textdisp()
{
	stdScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO stdInfo;
	GetConsoleScreenBufferInfo(stdScreenBuffer, &stdInfo);
	width = stdInfo.srWindow.Right - stdInfo.srWindow.Left + 1;
	height = stdInfo.srWindow.Bottom - stdInfo.srWindow.Top + 1;
	screen = new CHAR_INFO[width*height];
	clear();
	screenBuffer = CreateConsoleScreenBuffer( 
	   GENERIC_WRITE,
	   FILE_SHARE_WRITE,
	   NULL,
	   CONSOLE_TEXTMODE_BUFFER,
	   NULL
	);
	SetConsoleActiveScreenBuffer(screenBuffer);
}

Textdisp::Textdisp(int width, int height)
{
	this->width = width;
	this->height = height;
	screen = new CHAR_INFO[width*height];
	clear();
	stdScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	screenBuffer = CreateConsoleScreenBuffer( 
	   GENERIC_WRITE,
	   FILE_SHARE_WRITE,
	   NULL,
	   CONSOLE_TEXTMODE_BUFFER,
	   NULL
	);
	SetConsoleActiveScreenBuffer(screenBuffer);
}

Textdisp::~Textdisp()
{
	delete[] screen;
	SetConsoleActiveScreenBuffer(stdScreenBuffer);
	CloseHandle(stdScreenBuffer);
	CloseHandle(screenBuffer);
}

void Textdisp::clear()
{
	for (int i = 0; i < width*height; i++)
	{
		screen[i] = {{' '}, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY};
	}
}

void Textdisp::set(int x, int y, WCHAR c, WORD attributes)
{
	if (x >= 0 &&
		x < width &&
		y >= 0 &&
		y < height)
	{
		screen[x+y*width] = {{c}, attributes};
	}
}

WCHAR Textdisp::getChar(int x, int y)
{
	if (x >= 0 &&
		x < width &&
		y >= 0 &&
		y < height)
	{
		return screen[x+y*width].Char.UnicodeChar;
	}
	else
	{
		return 0;
	}
}

WORD Textdisp::getAttributes(int x, int y)
{
	if (x >= 0 &&
		x < width &&
		y >= 0 &&
		y < height)
	{
		return screen[x+y*width].Attributes;
	}
	else
	{
		return 0;
	}
}

void Textdisp::draw()
{
	SMALL_RECT destRect;
	destRect.Left = 0;
	destRect.Top = 0;
	destRect.Right = width;
	destRect.Bottom = height;
	WriteConsoleOutput(screenBuffer, screen, {(SHORT)width, (SHORT)height}, {0, 0}, &destRect);
}