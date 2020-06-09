#pragma once
#include "textutils.cpp"

struct MoveData
{
	bool w;
	bool a;
	bool s;
	bool d;
};

class Cursor
{
private:
	Box box;
public:
	int x;
	int y;

	Cursor(int boxWidth, int boxHeight);
	Cursor(Box box);

	//returnerar de håll som den faktiskt har rört sig i
	MoveData move(MoveData moveData);
};