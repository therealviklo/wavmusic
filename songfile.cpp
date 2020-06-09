#pragma once
#include "songfile.h"

void saveSong(const SongData& sd, std::string filename)
{
	File file(filename, "wb");

	//formatversion
	file.writeUnsigned<uint8_t>(2);
	//bpm
	file.writeUnsigned<uint16_t>(sd.bpm);
	//taktartssignatur
	file.writeUnsigned<uint8_t>(sd.timeSignature.top);
	file.writeUnsigned<uint8_t>(sd.timeSignature.bottom);
	//raster
	file.writeUnsigned<uint8_t>(sd.raster);

	//antalet spår
	file.writeUnsigned<uint8_t>(sd.songInfo.size() - 1);
	//instrumenten
	for (auto i = sd.songInfo.begin(); i != sd.songInfo.end(); i++)
	{
		for (auto c = i->instrumentName.begin(); c != i->instrumentName.end(); c++)
		{
			file.writeUnsigned<uint8_t>(*c);
		}
		file.writeUnsigned<uint8_t>('\0');
	}

	//skriver noter
	for (auto i = sd.song.begin(); i != sd.song.end(); i++)
	{
		file.writeUnsigned<uint8_t>(i->tone);
		file.writeUnsigned<uint32_t>(i->timestamp * 10000);
		file.writeUnsigned<uint32_t>(i->duration * 10000);
		file.writeUnsigned<uint8_t>(i->track);
	}
}

SongData loadSong(std::string filename, InstrumentHandler& instruments)
{
	File file(filename, "rb");
	SongData sd;

	uint8_t versionNumber = file.readUnsigned<uint8_t>();
	if (versionNumber == 1)
	{
		while (true)
		{
			try
			{
				uint8_t tone = file.readUnsigned<uint8_t>();
				uint32_t timestamp = file.readUnsigned<uint32_t>();
				uint32_t duration = file.readUnsigned<uint32_t>();
				uint8_t track = file.readUnsigned<uint8_t>();
				sd.song.push_back({tone, timestamp / 10000.0, duration / 10000.0, track});
			}
			catch (const std::runtime_error& e) //sluta när slutet nås
			{
				break;
			}
		}
		sd.bpm = 180;
		instruments.add("shamesque");
		sd.raster = 8;
		sd.songInfo = {{"shamesque"}};
		sd.timeSignature = {4, 4};
	}
	else if (versionNumber == 2)
	{
		sd.bpm = file.readUnsigned<uint16_t>();
		sd.timeSignature.top = file.readUnsigned<uint8_t>();
		sd.timeSignature.bottom = file.readUnsigned<uint8_t>();
		sd.raster = file.readUnsigned<uint8_t>();

		int numTracks = file.readUnsigned<uint8_t>() + 1;
		for (int i = 0; i < numTracks; i++)
		{
			std::string instrumentName;
			while (true)
			{
				char c = file.readUnsigned<uint8_t>();
				if (c == '\0')
				{
					break;
				}
				else
				{
					instrumentName += c;
				}
			}
			instruments.add(instrumentName);
			sd.songInfo.push_back({instrumentName});
		}
		while (true)
		{
			try
			{
				uint8_t tone = file.readUnsigned<uint8_t>();
				uint32_t timestamp = file.readUnsigned<uint32_t>();
				uint32_t duration = file.readUnsigned<uint32_t>();
				uint8_t track = file.readUnsigned<uint8_t>();
				sd.song.push_back({tone, timestamp / 10000.0, duration / 10000.0, track});
			}
			catch (const std::runtime_error& e) //sluta när slutet nås
			{
				break;
			}
		}
	}
	else
	{
		throw std::runtime_error("Cannot read song file format");
	}
	return sd;
}