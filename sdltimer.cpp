#pragma once
#include "sdltimer.h"

Timer::Timer(int framerate)
{
	this->framerate = framerate;
}

void Timer::start()
{
	startTime = SDL_GetTicks();
}

void Timer::wait()
{
	double ms = 1000.0/framerate-(SDL_GetTicks()-startTime);
	if (ms > 0 && ms <= 1000.0 / framerate) SDL_Delay(ms);
}

void Timer::staticWait()
{
	SDL_Delay(1000.0/framerate);
}