#pragma once
#include "timer.h"

Timer::Timer(int framerate)
{
	this->waitTime = 1.0 / framerate;
}

void Timer::start()
{
	startTime = clock();
}

void Timer::wait()
{
	double s = waitTime - (clock() - startTime) / CLOCKS_PER_SEC;
	if (s > 0 && s <= waitTime) Sleep(s * 1000.0);
}

void Timer::staticWait()
{
	Sleep(1000.0 * waitTime);
}