#pragma once
#include "SDL2/SDL.h"

class Timer
{
private:
	Uint32 startTime;
public:
	int framerate;

	Timer(int framerate);

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	void start();
	void wait();
	void staticWait();
};