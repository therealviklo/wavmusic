#pragma once
#include "song.cpp"
#include "file.cpp"
#include "songdata.cpp"

/* 
 * FORMAT VERSION 1:
 * Först 8 bitar, inget tecken, för versionsnumret, som är 1.
 * Sedan alla noter på rad.
 * En not ser ut på detta sätt:
 * - 8 bitar, inget tecken, för tonen.
 * - 32 bitar, inget tecken, för timestampen. (Enhet är tiotusendels helnoter.)
 * - 32 bitar, inget tecken, för längden. (Enhet är tiotusendels helnoter.)
 * - 8 bitar, inget tecken, för spårnumret.
 * 
 * FORMAT VERSION 2:
 * Först 8 bitar, inget tecken, för versionsnumret, som är 2.
 * Sedan 16 bitar, inget tecken, för bpm:en.
 * Sedan 2 byte utan tecken för taktartssignaturen, den första för täljaren och den andra för nämnaren.
 * Sedan 8 bitar, inget tecken, för rastern.
 * Sedan 8 bitar utan tecken för antalet spår MINUS 1, d.v.s. 0 betyder ett spår.
 * Sedan en nullterminerad sträng för varje spår som berättar filnamnet på instrumentet för det spåret.
 * Till sist alla noter på rad.
 * En not ser ut på detta sätt:
 * - 8 bitar, inget tecken, för tonen.
 * - 32 bitar, inget tecken, för timestampen. (Enhet är tiotusendels helnoter.)
 * - 32 bitar, inget tecken, för längden. (Enhet är tiotusendels helnoter.)
 * - 8 bitar, inget tecken, för spårnumret.
*/ 

void saveSong(const SongData& song, const std::string& filename);
SongData loadSong(const std::string& filename, InstrumentHandler& instruments);