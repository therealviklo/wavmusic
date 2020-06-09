#pragma once
#include "song.cpp"
#include <map>

class InstrumentHandler
{
private:
	std::map<std::string, std::unique_ptr<Instrument>> instruments;
public:
	Instrument& operator[](std::string name);
	void add(std::string name);
};