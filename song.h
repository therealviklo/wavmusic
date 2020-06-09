#pragma once
#include "wave.cpp"
#include <vector>
#include <memory>
#include <cmath>
#include <list>
#include <map>

struct Note
{
	int tone;
	double timestamp;
	double duration;
	int track;
};

typedef std::list<Note> Song;

double songLength(const Song& song);

struct Envelope
{
	double attack;
	double decay;
	double sustain;
	double release;
};

class Instrument
{
private:
	std::unique_ptr<Wave> sounds[9*12];
	Envelope envelope;

	void readEnvelope(std::string name);
public:
	Instrument(std::string name);
	Instrument(Wave& wave, Envelope envelope);

	Instrument(const Instrument&) = delete;
	Instrument& operator=(const Instrument&) = delete;

	void play(Wave& wave, Note note, double bpm);
};

// typedef std::map<std::string, std::unique_ptr<Instrument>> Instruments_t;

struct TrackInfo
{
	// Instruments_t::iterator instrument;
	std::string instrumentName;
};

typedef std::vector<TrackInfo> SongInfo;