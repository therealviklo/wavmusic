#pragma once
#include "wave.h"

Wave::Wave(int numChannels, int samplingRate, int length, bool initialise)
{
	if (numChannels != 1 && numChannels != 2) throw std::runtime_error("Wave is neither mono nor stereo");
	this->length = length;
	this->samplingRate = samplingRate;
	for (int i = 0; i < numChannels; i++)
	{
		channels.push_back(new int32_t[length]);
		if (initialise)
		{
			for (int j = 0; j < length; j++)
			{
				channels[i][j] = 0;
			}
		}
	}
}

Wave::Wave(const Wave& o)
{
	this->length = o.length;
	this->samplingRate = o.samplingRate;
	for (int i = 0; i < o.channels.size(); i++)
	{
		channels.push_back(new int32_t[length]);
		for (int j = 0; j < length; j++)
		{
			channels[i][j] = o.channels[i][j];
		}
	}
}

Wave::~Wave()
{
	for (auto i = channels.begin(); i != channels.end(); i++)
	{
		delete[] *i;
	}
}

Wave& Wave::operator=(const Wave& o)
{
	if (this != &o)
	{
		for (auto i = channels.begin(); i != channels.end(); i++)
		{
			delete[] *i;
		}

		this->length = o.length;
		this->samplingRate = o.samplingRate;
		for (int i = 0; i < o.channels.size(); i++)
		{
			channels.push_back(new int32_t[length]);
			for (int j = 0; j < length; j++)
			{
				channels[i][j] = o.channels[i][j];
			}
		}
	}
	return *this;
}

int32_t Wave::at(int channel, double time)
{
	return channels.at(channel)[int(time*samplingRate)] + (time*samplingRate - int(time*samplingRate))*(channels.at(channel)[int(time*samplingRate)+1] - channels.at(channel)[int(time*samplingRate)]);
}

void Wave::write(std::string filename, int numBytes)
{
	File file(filename, "wb");

	//RIFF-header
	file.writeString("RIFF");
	file.write<int32_t, true>(36 + length*2*channels.size());
	file.writeString("WAVE");

	//fmt 
	file.writeString("fmt ");
	file.write<int32_t, true>(16);
	file.write<int16_t, true>(1);
	file.write<int16_t, true>(channels.size());
	file.write<int32_t, true>(samplingRate);
	file.write<int32_t, true>(samplingRate * channels.size() * numBytes);
	file.write<int16_t, true>(channels.size() * numBytes);
	file.write<int16_t, true>(8 * numBytes);

	//data
	file.writeString("data");
	file.write<int32_t, true>(length * numBytes * channels.size());
	for (int i = 0; i < length; i++)
	{
		for (auto j = channels.begin(); j != channels.end(); j++)
		{
			if (numBytes == 1)
			{
				file.write<uint32_t, true>(((*j)[i] / (double)maxValue(4) * maxValue(numBytes)) + 128, 1);
			}
			else
			{
				file.write<int32_t, true>((*j)[i] / (double)maxValue(4) * maxValue(numBytes), numBytes);
			}
		}
	}
}

void Wave::add(Wave& o, double time)
{
	if (channels.size() >= o.channels.size())
	{
		for (int i = time*samplingRate; i < length && int((i - time*samplingRate)*(o.samplingRate/(double)samplingRate)) + 1 < o.length; i++)
		{
			if (i >= 0)
			{
				for (int j = 0; j < channels.size(); j++)
				{
					channels[j][i] += o.at(o.channels.size()-1 > j ? j : o.channels.size()-1, (i - time*samplingRate)*(o.samplingRate/(double)samplingRate)/o.samplingRate);
				}
			}
		}
	}
	else
	{
		for (int i = time*samplingRate; i < length && int((i - time*samplingRate)*(o.samplingRate/(double)samplingRate)) + 1 < o.length; i++)
		{
			if (i >= 0)
			{
				channels[0][i] += 
					o.at(0, (i - time*samplingRate)*(o.samplingRate/(double)samplingRate)/o.samplingRate)/2.0 +
					o.at(1, (i - time*samplingRate)*(o.samplingRate/(double)samplingRate)/o.samplingRate)/2.0;
			}
		}
	}
}

void Wave::copy(Wave& o, double time)
{
	if (channels.size() >= o.channels.size())
	{
		for (int i = time*samplingRate; i < length && int((i - time*samplingRate)*(o.samplingRate/(double)samplingRate)) + 1 < o.length; i++)
		{
			if (i >= 0)
			{
				for (int j = 0; j < channels.size(); j++)
				{
					channels[j][i] = o.at(o.channels.size()-1 > j ? j : o.channels.size()-1, (i - time*samplingRate)*(o.samplingRate/(double)samplingRate)/o.samplingRate);
				}
			}
		}
	}
	else
	{
		for (int i = time*samplingRate; i < length && int((i - time*samplingRate)*(o.samplingRate/(double)samplingRate)) + 1 < o.length; i++)
		{
			if (i >= 0)
			{
				channels[0][i] = 
					o.at(0, (i - time*samplingRate)*(o.samplingRate/(double)samplingRate)/o.samplingRate)/2.0 +
					o.at(1, (i - time*samplingRate)*(o.samplingRate/(double)samplingRate)/o.samplingRate)/2.0;
			}
		}
	}
}

void Wave::fade(double startTime, double startAmplitude, double endTime, double endAmplitude)
{
	for (int i = startTime*samplingRate; i < int(endTime*samplingRate) && i < length; i++)
	{
		if (i >= 0)
		{
			for (int j = 0; j < channels.size(); j++)
			{
				channels[j][i] = channels[j][i] * (startAmplitude + (endAmplitude - startAmplitude) * (i - startTime * samplingRate) / ((endTime - startTime) * samplingRate));
			}
		}
	}
}

Wave Wave::changeSpeed(double factor)
{
	Wave wave(channels.size(), samplingRate, length/factor, false);
	for (int i = 0; i < wave.length; i++)
	{
		for (int j = 0; j < wave.channels.size(); j++)
		{
			wave.channels[j][i] = at(j, i*factor/samplingRate);
		}
	}
	return wave;
}

Wave read(std::string filename)
{
	File file(filename, "rb");

	//RIFF-header
	std::string chunkId = file.readString(4);
	if (chunkId != "RIFF") throw std::runtime_error("Header is not RIFF");
	file.read<int32_t, true>(); //förkasta storlek
	std::string formatName = file.readString(4);
	if (formatName != "WAVE") throw std::runtime_error("Format is not WAVE");

	//skippa till fmt 
	while (file.readString(4) != "fmt ")
	{
		int32_t subchunkSize = file.read<int32_t, true>();
		file.skip(subchunkSize);
	}

	//fmt 
	//har redan läst in id:t
	int32_t subchunk1Size = file.read<int32_t, true>();
	if (subchunk1Size != 16) throw std::runtime_error("Unsupported audio format");
	int16_t audioFormat = file.read<int16_t, true>();
	if (audioFormat != 1) throw std::runtime_error("Unsupported audio format");
	int16_t numChannels = file.read<int16_t, true>();
	if (numChannels != 1 && numChannels != 2) throw std::runtime_error("Audio is neither mono nor stereo");
	int32_t samplingRate = file.read<int32_t, true>();
	int32_t byteRate = file.read<int32_t, true>();
	int16_t blockAlign = file.read<int16_t, true>();
	int16_t bitsPerSample = file.read<int16_t, true>();
	if (bitsPerSample > 32) throw std::runtime_error("Bit depth is too high");

	//skippa till data
	while (file.readString(4) != "data")
	{
		int32_t subchunkSize = file.read<int32_t, true>();
		file.skip(subchunkSize);
	}

	//data
	//har redan läst in id:t
	int32_t subchunk2Size = file.read<int32_t, true>();
	Wave wave(numChannels, samplingRate, subchunk2Size / blockAlign);
	for (int i = 0; i < subchunk2Size / blockAlign; i++)
	{
		for (int j = 0; j < numChannels; j++)
		{
			if (bitsPerSample == 8)
			{
				wave.channel(j)[i] = (int32_t(file.readUnsigned<uint32_t, true>(bitsPerSample / 8)) - 128) / (double)maxValue(bitsPerSample / 8) * maxValue(4);
			}
			else
			{
				wave.channel(j)[i] = file.read<int32_t, true>(bitsPerSample / 8) / (double)maxValue(bitsPerSample / 8) * maxValue(4);
			}
		}
	}
	return wave;
}