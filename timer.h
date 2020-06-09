#pragma once
#include <windows.h>
#include <ctime>

class Timer
{
private:
	clock_t startTime;
	double waitTime;
public:
	Timer(int framerate);

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	void start();
	void wait();
	void staticWait();
};