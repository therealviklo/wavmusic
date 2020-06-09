#pragma once
#include "instrumenthandler.h"

Instrument& InstrumentHandler::operator[](std::string name)
{
	add(name);
	return *instruments.at(name);
}

void InstrumentHandler::add(std::string name)
{
	if (!instruments.count(name))
	{
		instruments.emplace(name, std::make_unique<Instrument>(".\\inst\\" + name));
	}
}