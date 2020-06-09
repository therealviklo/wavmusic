#pragma once

struct Ratio
{
	int top;
	int bottom;
};

template <typename T>
T min(const T& a, const T& b);

template <typename T>
T max(const T& a, const T& b);