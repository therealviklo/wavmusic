#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>
#include <sstream>
#include <memory>

class Sound
{
private:
	std::string name;
public:
	Sound() {name = "";}
	Sound(std::string path, std::string alias);
	Sound(Sound&& o);
	~Sound();

	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;

	void play(double timestamp = 0);
	void stop();

	std::string getName() {return name;}
	bool isPlaying();

	operator bool();
};

namespace MM
{
	std::unique_ptr<Sound> currentlyPlaying;

	void playSound(std::string filename, double timestamp = 0);
	void stopSound();
	bool isPlaying();
}