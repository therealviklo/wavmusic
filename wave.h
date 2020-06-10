#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "file.cpp"

class Wave
{
private:
	int length;
	int samplingRate;
	std::vector<int32_t*> channels;
public:
	Wave(int numChannels, int samplingRate, int length, bool initialise = true);
	Wave(const Wave& o);
	~Wave();

	Wave& operator=(const Wave& o);
	
	int getNumChannels() {return channels.size();}
	int32_t* channel(int channel) {return channels.at(channel);}
	int getLength() {return length;}
	int getSamplingRate() {return samplingRate;}

	int32_t at(int channel, double time);

	void write(const std::string& filename, int numBytes = 2);

	void add(Wave& o, double time = 0);
	void copy(Wave& o, double time = 0);
	void fade(double startTime, double startAmplitude, double endTime, double endAmplitude);

	Wave changeSpeed(double factor);
};

Wave read(const std::string& filename);