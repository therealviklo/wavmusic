#pragma once
#include "keyboard.h"

namespace KB
{
	// Keyboard KB(10, 1, 0.5);
	Textinp KB;
}

// Keyboard::Keyboard(int interval, int minInterval, double decrease)
// {
// 	this->interval = interval;
// 	this->minInterval = minInterval;
// 	this->decrease = decrease;
// }

// bool Keyboard::pressed(int c)
// {
// 	if (!(bool)keys.count(c))
// 	{
// 		keys.emplace(c, SinglePressKey(c, interval, minInterval, decrease));
// 	}
// 	return keys.at(c).pressed();
// }

// bool Keyboard::down(int c)
// {
// 	if (!(bool)keys.count(c))
// 	{
// 		keys.emplace(c, SinglePressKey(c, interval, minInterval, decrease));
// 	}
// 	return keys.at(c).down();
// }