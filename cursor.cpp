#pragma once
#include "cursor.h"

Cursor::Cursor(int boxWidth, int boxHeight)
{
	box = {0, 0, boxWidth-1, boxHeight-1};
	x = 0;
	y = 0;
}

Cursor::Cursor(Box box)
{
	this->box = box;
	x = 0;
	y = 0;
}

MoveData Cursor::move(MoveData moveData)
{
	if (moveData.w) y--;
	if (moveData.a) x--;
	if (moveData.s) y++;
	if (moveData.d) x++;

	if (x < box.left)
	{
		x = box.left;
		moveData.a = false;
	}
	if (x > box.right)
	{
		x = box.right;
		moveData.d = false;
	}
	if (y < box.top)
	{
		y = box.top;
		moveData.w = false;
	}
	if (y > box.bottom)
	{
		y = box.bottom;
		moveData.s = false;
	}
	
	return moveData;
}