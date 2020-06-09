#pragma once
#include "multimedia.h"

Sound::Sound(std::string path, std::string alias)
{
	this->name = alias;
	if (name != "")
	{
		std::string command = "open \"" + path + "\" type waveaudio alias \"" + alias + "\"";
		if (mciSendStringA(command.c_str(), NULL, 0, NULL)) throw std::runtime_error("MCI error when loading sound");
		mciSendStringA(("set \"" + alias + "\" time format ms").c_str(), NULL, 0, NULL);
	}
}

Sound::Sound(Sound&& o)
{
	name = o.name;
	o.name = "";
}

Sound::~Sound()
{
	if (name != "")
	{
		std::string command = "close \"" + name + "\"";
		mciSendStringA(command.c_str(), NULL, 0, NULL);
	}
}

void Sound::play(double timestamp)
{
	if (name != "")
	{
		std::stringstream command;
		command << "play \"";
		command << name;
		command << "\" from ";
		command << int(timestamp * 1000);
		if (mciSendStringA(command.str().c_str(), NULL, 0, NULL)) throw std::runtime_error("MCI error when playing sound");
	}
}

void Sound::stop()
{
	if (name != "")
	{
		std::string command = "stop \"" + name + "\"";
		if (mciSendStringA(command.c_str(), NULL, 0, NULL)) throw std::runtime_error("MCI error when stopping sound");
	}
}

bool Sound::isPlaying()
{
	if (name != "")
	{
		char pos[256];
		if (mciSendStringA(("status " + name + " position").c_str(), pos, sizeof(pos) / sizeof(char), NULL))
			throw std::runtime_error("MCI error when getting sound position");
		char len[256];
		if (mciSendStringA(("status " + name + " length").c_str(), len, sizeof(len) / sizeof(char), NULL))
			throw std::runtime_error("MCI error when getting sound length");
		return std::stoi(pos) < std::stoi(len);
	}
	else
	{
		return false;
	}
}

Sound::operator bool()
{
	return name != "";
}

namespace MM
{
	void playSound(std::string filename, double timestamp)
	{
		stopSound();
		currentlyPlaying = std::make_unique<Sound>(filename, filename);
		currentlyPlaying->play(timestamp);
	}

	void stopSound()
	{
		if (currentlyPlaying)
		{
			currentlyPlaying->stop();
			currentlyPlaying.reset();
		}
	}

	bool isPlaying()
	{
		if (currentlyPlaying)
		{
			if (currentlyPlaying->isPlaying())
			{
				return true;
			}
			else
			{
				currentlyPlaying.reset();
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}