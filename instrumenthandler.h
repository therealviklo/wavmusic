#pragma once
#include "song.cpp"
#include <map>

class InstrumentHandler
{
private:
	std::map<std::string, std::unique_ptr<Instrument>> instruments;
public:
	Instrument& operator[](const std::string& name);
	void add(const std::string& name);
};