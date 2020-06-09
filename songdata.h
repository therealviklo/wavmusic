#pragma once
#include "utils.cpp"
#include "song.cpp"
#include "instrumenthandler.cpp"

struct SongData
{
	short bpm;
	Ratio timeSignature;
	short raster;
	Song song;
	SongInfo songInfo;
};