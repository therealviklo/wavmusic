#pragma once
#include "song.h"

double songLength(const Song& song)
{
	double highestYet = 0;
	for (auto i = song.cbegin(); i != song.cend(); i++)
	{
		if (i->timestamp + i->duration > highestYet)
		{
			highestYet = i->timestamp + i->duration;
		}
	}
	return highestYet;
}

void Instrument::readEnvelope(const std::string& name)
{
	File envelopeFile(name + "\\envelope.txt", "r");
	int c = '\0';
	std::string attack;
	while (true)
	{
		c = envelopeFile.get();
		if (c == '\n') break;
		attack += c;
	}
	envelope.attack = std::stod(attack);
	std::string decay;
	while (true)
	{
		c = envelopeFile.get();
		if (c == '\n') break;
		decay += c;
	}
	envelope.decay = std::stod(decay);
	std::string sustain;
	while (true)
	{
		c = envelopeFile.get();
		if (c == '\n') break;
		sustain += c;
	}
	envelope.sustain = std::stod(sustain);
	std::string release;
	while (true)
	{
		try
		{
			c = envelopeFile.get();
		}
		catch (std::runtime_error)
		{
			break;
		}
		if (c == '\n') break;
		release += c;
	}
	envelope.release = std::stod(release);
}

Instrument::Instrument(const std::string& name)
{
	readEnvelope(name);
	Wave original = read(name + "\\sample.wav");
	for (int i = 0; i < 9*12; i++)
	{
		sounds[i] = std::make_unique<Wave>(original.changeSpeed(pow(2, (i - 4*12)/12.0)));
	}
}

Instrument::Instrument(Wave& wave, Envelope envelope)
{
	this->envelope = envelope;
	for (int i = 0; i < 9*12; i++)
	{
		sounds[i] = std::make_unique<Wave>(wave.changeSpeed(pow(2, (i - 4*12)/12.0)));
	}
}

void Instrument::play(Wave& wave, Note note, double bpm)
{
	if (note.tone >= 0 && note.tone < 9*12)
	{
		Wave sound(sounds[note.tone]->getNumChannels(), sounds[note.tone]->getSamplingRate(), (note.duration * 240.0 / bpm + envelope.release) * sounds[note.tone]->getSamplingRate(), true);
		sound.copy(*sounds[note.tone]);
		sound.fade(0, 0, envelope.attack, 1);
		sound.fade(envelope.attack, 1, envelope.attack + envelope.decay, 0);
		sound.fade(envelope.attack + envelope.decay, envelope.sustain, note.duration * 240.0 / bpm + envelope.release, envelope.sustain);
		sound.fade(note.duration * 240.0 / bpm, 1, note.duration * 240.0 / bpm + envelope.release, 0);
		wave.add(sound, note.timestamp * 240.0 / bpm);
	}
}