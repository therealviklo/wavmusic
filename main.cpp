#include <iostream>
#include <cstdint>
#include <windows.h>
#include "wave.cpp"
#include "file.cpp"
#include "song.cpp"
#include "textdisp.cpp"
#include "cursor.cpp"
#include "sdltimer.cpp"
#include "utils.cpp"
#include "filedialogue.cpp"
#include "textmessagebox.cpp"
#include "foldersearch.cpp"
#include "multimedia.cpp"
#include "menu.cpp"
#include "keyboard.cpp"
#include "songfile.cpp"
#include "instrumenthandler.cpp"
#include "songdata.cpp"

const char defaultInstrument[] = "shamesque";

template <typename T>
bool elementInList(const T& t, const std::list<T>& list)
{
	for (auto i = list.cbegin(); i != list.cend(); i++)
	{
		if (*i == t)
		{
			return true;
		}
	}
	return false;
}

void output(Song& song, SongInfo& songInfo, int bpm, InstrumentHandler& instruments, std::string filename)
{
	Wave wave(1, 48000, 48000 * songLength(song) * 240.0 / bpm);
	for (auto i = song.begin(); i != song.end(); i++)
	{
		instruments[songInfo.at(i->track).instrumentName].play(wave, *i, bpm);
	}
	wave.write(filename);
}

#undef main

int main(int argc, char* argv[])//int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		SetConsoleCP(437);

		Timer timer(60);

		Textdisp textdisp;
		bool running = true;

		Box pianoRollBox = {0, 1, textdisp.getWidth() - 1, textdisp.getHeight() - 1};
		Menu menu({
			{
				"File",
				{
					"Export",
					"Save",
					"Load"
				}
			},
			{
				"Song",
				{
					"BPM"
				}
			},
			{
				"Track",
				{
					"Instrument"
				}
			},
			{
				"Raster",
				{
					"4",
					"8",
					"16",
					"32"
				}
			}
		});

		Cursor cursor(pianoRollBox);
		int horizontalScroll = 0;
		int verticalScroll = 3*12;

		SongData sd;
		sd.bpm = 180;
		sd.timeSignature = {4, 4};
		sd.raster = 8;
		sd.songInfo = {{defaultInstrument}};
		
		InstrumentHandler instruments;
		instruments.add(defaultInstrument);
		
		int currentTrack = 0;
		std::list<Song::iterator> selectedNotes;
		std::unique_ptr<Point> boxSelectStart;

		// Wave defaultInstrumentWave(1, 48000, 48000*10, false);
		// for (int i = 0; i < defaultInstrumentWave.getLength(); i++)
		// {
		//     defaultInstrumentWave.channel(0)[i] = sin(i / 440.0 * 3.141592 * 16) * 0x2fffffff;
		// }

		while (running)
		{
			timer.start();

			//input
			if (KB::KB.pressed('X'))
			{
				running = false;
				break;
			}
			MoveData pressedKeys = {KB::KB.typed('W'), KB::KB.typed('A'), KB::KB.typed('S'), KB::KB.typed('D')};
			MoveData actualMoveDirections = cursor.move(pressedKeys);
			if (actualMoveDirections.w)
			{
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					(**i).tone += 1;
				}
			}
			else if (pressedKeys.w)
			{
				if (verticalScroll < 9*12 - (pianoRollBox.bottom - pianoRollBox.top + 1))
				{
					verticalScroll += 1;
					for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
					{
						(**i).tone += 1;
					}
				}
			}
			if (actualMoveDirections.a)
			{
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					(**i).timestamp -= 1.0 / sd.raster;
				}
			}
			else if (pressedKeys.a)
			{
				if (horizontalScroll > 0)
				{
					horizontalScroll -= 1;
					for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
					{
						(**i).timestamp -= 1.0 / sd.raster;
					}
				}
			}
			if (actualMoveDirections.s)
			{
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					(**i).tone -= 1;
				}
			}
			else if (pressedKeys.s)
			{
				if (verticalScroll > 0)
				{
					verticalScroll -= 1;
					for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
					{
						(**i).tone -= 1;
					}
				}
			}
			if (actualMoveDirections.d)
			{
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					(**i).timestamp += 1.0 / sd.raster;
				}
			}
			else if (pressedKeys.d)
			{
				horizontalScroll += 1;
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					(**i).timestamp += 1.0 / sd.raster;
				}
			}
			if (KB::KB.pressed(VK_SHIFT) && !boxSelectStart)
			{
				if (!boxSelectStart)
				{
					boxSelectStart = std::make_unique<Point>();
				}
				boxSelectStart->x = cursor.x + horizontalScroll;
				boxSelectStart->y = pianoRollBox.bottom - cursor.y + verticalScroll;
				selectedNotes.clear();
			}
			if (!KB::KB.down(VK_SHIFT) && boxSelectStart)
			{
				for (auto i = sd.song.begin(); i != sd.song.end(); i++)
				{
					if (i->track == currentTrack)
					{
						int minX = min(cursor.x + horizontalScroll, boxSelectStart->x);
						int maxX = max(cursor.x + horizontalScroll, boxSelectStart->x);
						int minY = min(pianoRollBox.bottom - cursor.y + verticalScroll, boxSelectStart->y);
						int maxY = max(pianoRollBox.bottom - cursor.y + verticalScroll, boxSelectStart->y);
						if (
							i->tone >= minY &&
							i->tone <= maxY &&
							(
								(
									i->timestamp * sd.raster >= minX &&
									i->timestamp * sd.raster <= maxX
								) ||
								(
									(i->timestamp + i->duration) * sd.raster - 1 >= minX &&
									(i->timestamp + i->duration) * sd.raster - 1 <= maxX
								) ||
								(
									(
										i->timestamp * sd.raster <= minX &&
										(i->timestamp + i->duration) * sd.raster - 1 >= minX
									) ||
									(
										i->timestamp * sd.raster <= maxX &&
										(i->timestamp + i->duration) * sd.raster - 1 >= maxX
									)
								)
							)
						)
						{
							selectedNotes.push_back(i);
						}
					}
				}
				boxSelectStart.reset();
			}
			if (KB::KB.typed(VK_UP))
			{
				if (currentTrack + 1 < sd.songInfo.size())
				{
					currentTrack++;
					for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
					{
						(**i).track = currentTrack;
					}
				}
				else
				{
					if (promptTextMessageBoxA437("New track", "Create new track?", {"Yes", "No"}, textdisp, timer) == 0)
					{
						sd.songInfo.push_back({defaultInstrument});
						currentTrack++;
						for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
						{
							(**i).track = currentTrack;
						}
					}
				}
			}
			if (KB::KB.typed(VK_DOWN))
			{
				if (currentTrack > 0)
				{
					currentTrack--;
					for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
					{
						(**i).track = currentTrack;
					}
				}
			}
			if (KB::KB.typed(VK_LEFT))
			{
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					if ((**i).duration - 1.0 / sd.raster > 0)
					{
						(**i).duration -= 1.0 / sd.raster;
					}
				}
			}
			if (KB::KB.typed(VK_RIGHT))
			{
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					(**i).duration += 1.0 / sd.raster;
				}
			}
			if (KB::KB.pressed(VK_RETURN))
			{
				if (selectedNotes.size() > 0)
				{
					selectedNotes.clear();
				}
				else
				{
					for (auto i = sd.song.begin(); i != sd.song.end(); i++)
					{
						if (i->track == currentTrack)
						{
							if (pianoRollBox.bottom - i->tone + verticalScroll == cursor.y && i->timestamp * sd.raster <= cursor.x + horizontalScroll && (i->timestamp + i->duration) * sd.raster > cursor.x + horizontalScroll)
							{
								selectedNotes.clear();
								selectedNotes.push_back(i);
							}
						}
					}
				}
			}
			if (KB::KB.pressed(VK_SPACE))
			{
				sd.song.push_front({pianoRollBox.bottom - cursor.y + verticalScroll, (cursor.x + horizontalScroll) / (double)sd.raster, 1.0 / sd.raster, currentTrack});
				selectedNotes.clear();
				selectedNotes.push_back(sd.song.begin());
			}
			if (KB::KB.pressed(VK_BACK))
			{
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					sd.song.erase(*i);
				}
				selectedNotes.clear();
			}
			if (KB::KB.pressed('C'))
			{
				for (auto i = selectedNotes.begin(); i != selectedNotes.end(); i++)
				{
					sd.song.push_front(**i);
				}
			}
			if (KB::KB.pressed('K'))
			{
				try
				{
					if (MM::isPlaying())
					{
						MM::stopSound();
					}
					else if ((cursor.x + horizontalScroll) / (double)sd.raster < songLength(sd.song))
					{
						output(sd.song, sd.songInfo, sd.bpm, instruments, "tmp.wav");
						MM::playSound("tmp.wav", (cursor.x + horizontalScroll) / (double)sd.raster);
					}
				}
				catch (const std::exception& e)
				{
					promptTextMessageBoxA437("Error when playing song", e.what(), {"OK"}, textdisp, timer);
				}
				catch (...)
				{
					promptTextMessageBoxA437("Error when playing song", "Unknown error when playing song", {"OK"}, textdisp, timer);
				}
			}
			if (KB::KB.pressed(VK_MENU))
			{
				MenuResult menuResult = menu.enterMenu(0, 0, textdisp, timer);
				if (menuResult.successful)
				{
					if (menuResult.menu == "File")
					{
						if (menuResult.option == "Export")
						{
							std::string inputFilename = promptTextInputBoxA437("Filename?", textdisp, timer);
							int inputFilenameSize = inputFilename.length();
							if (!(inputFilenameSize >= 4 &&
								inputFilename[inputFilenameSize - 4] == '.' &&
								(inputFilename[inputFilenameSize - 3] == 'w' || inputFilename[inputFilenameSize - 3] == 'W') &&
								(inputFilename[inputFilenameSize - 2] == 'a' || inputFilename[inputFilenameSize - 2] == 'A') &&
								(inputFilename[inputFilenameSize - 1] == 'w' || inputFilename[inputFilenameSize - 1] == 'W')))
							{
								inputFilename += ".wav";
							}
							output(sd.song, sd.songInfo, sd.bpm, instruments, ".\\wav\\" + inputFilename);
						}
						else if (menuResult.option == "Save")
						{
							std::string inputFilename = promptTextInputBoxA437("Filename?", textdisp, timer);
							int inputFilenameSize = inputFilename.length();
							if (!(inputFilenameSize >= 4 &&
								inputFilename[inputFilenameSize - 4] == '.' &&
								(inputFilename[inputFilenameSize - 3] == 'c' || inputFilename[inputFilenameSize - 3] == 'C') &&
								(inputFilename[inputFilenameSize - 2] == 'm' || inputFilename[inputFilenameSize - 2] == 'M') &&
								(inputFilename[inputFilenameSize - 1] == 'e' || inputFilename[inputFilenameSize - 1] == 'E')))
							{
								inputFilename += ".cme";
							}
							saveSong(sd, ".\\cme\\" + inputFilename);
						}
						else if (menuResult.option == "Load")
						{
							std::string inputFilename = promptTextInputBoxA437("Filename?", textdisp, timer);
							int inputFilenameSize = inputFilename.length();
							if (!(inputFilenameSize >= 4 &&
								inputFilename[inputFilenameSize - 4] == '.' &&
								(inputFilename[inputFilenameSize - 3] == 'c' || inputFilename[inputFilenameSize - 3] == 'C') &&
								(inputFilename[inputFilenameSize - 2] == 'm' || inputFilename[inputFilenameSize - 2] == 'M') &&
								(inputFilename[inputFilenameSize - 1] == 'e' || inputFilename[inputFilenameSize - 1] == 'E')))
							{
								inputFilename += ".cme";
							}
							sd = loadSong(".\\cme\\" + inputFilename, instruments);
						}
					}
					else if (menuResult.menu == "Song")
					{
						if (menuResult.option == "BPM")
						{
							std::string inputBPM = promptTextInputBoxA437("Input BPM", textdisp, timer);
							try
							{
								double doubleBPM = std::stod(inputBPM);
								if (!std::isinf(doubleBPM) && !std::isnan(doubleBPM) && doubleBPM > 0)
								{
									sd.bpm = doubleBPM;
								}
								else
								{
									promptTextMessageBoxA437("Error", "Invalid BPM", {"OK"}, textdisp, timer);
								}
							}
							catch (std::invalid_argument)
							{
								promptTextMessageBoxA437("Error", "Invalid BPM", {"OK"}, textdisp, timer);
							}
							catch (std::out_of_range)
							{
								promptTextMessageBoxA437("Error", "Invalid BPM", {"OK"}, textdisp, timer);
							}
						}
					}
					else if (menuResult.menu == "Track")
					{
						if (menuResult.option == "Instrument")
						{
							std::string inputInstrument = promptTextInputBoxA437("Instrument name?", textdisp, timer);
							try
							{
								instruments.add(inputInstrument);
								sd.songInfo.at(currentTrack).instrumentName = inputInstrument;
							}
							catch (const std::runtime_error& e)
							{
								promptTextMessageBoxA437("Error when setting instrument", e.what(), {"OK"}, textdisp, timer);
							}
						}
					}
					else if (menuResult.menu == "Raster")
					{
						if (menuResult.option == "4")
						{
							sd.raster = 4;
						}
						else if (menuResult.option == "8")
						{
							sd.raster = 8;
						}
						else if (menuResult.option == "16")
						{
							sd.raster = 16;
						}
						else if (menuResult.option == "32")
						{
							sd.raster = 32;
						}
					}
				}
			}
			if (KB::KB.pressed(VK_HOME))
			{
				horizontalScroll = 0;
				cursor.x = pianoRollBox.left;
			}
			
			//rita
			textdisp.clear();

			//ritar taktmarkeringar
			for (int i = horizontalScroll; i - horizontalScroll <= pianoRollBox.right - pianoRollBox.left; i++)
			{
				if (i % (sd.raster * sd.timeSignature.top / sd.timeSignature.bottom) == 0)
				{
					for (int j = pianoRollBox.top; j <= pianoRollBox.bottom; j++)
					{
						textdisp.set(pianoRollBox.left + i - horizontalScroll, j, ' ', BackgroundColour::darkgrey | ForegroundColour::white);
					}
				}
			}
			//ritar C-markeringar
			for (int i = 0; i < 9; i++)
			{
				if (pianoRollBox.bottom - i*12 - 3 + verticalScroll >= pianoRollBox.top && pianoRollBox.bottom - i*12 - 3 + verticalScroll <= pianoRollBox.bottom)
				{
					for (int j = pianoRollBox.left; j <= pianoRollBox.right; j++)
					{
						textdisp.set(j, pianoRollBox.bottom - i*12 - 3 + verticalScroll, j == 0 ? 'C' : j == 1 ? '0' + i : ' ', BackgroundColour::darkgrey | ForegroundColour::black);
					}
				}
			}

			//ritar noter
			for (auto i = sd.song.begin(); i != sd.song.end(); i++)
			{
				if (i->track == currentTrack &&
					pianoRollBox.bottom - i->tone + verticalScroll <= pianoRollBox.bottom &&
					pianoRollBox.bottom - i->tone + verticalScroll >= pianoRollBox.top)
				{
					for (int j = 0; j < i->duration * sd.raster; j++)
					{
						textdisp.set(i->timestamp * sd.raster + j - horizontalScroll, pianoRollBox.bottom - i->tone + verticalScroll, j == 0 ? '#' : '-', (elementInList(i, selectedNotes) ? BackgroundColour::darkyellow : BackgroundColour::white) | ForegroundColour::black);
					}
				}
			}

			//ritar rektangelmarkering
			if (boxSelectStart)
			{
				int minX = min(cursor.x, boxSelectStart->x - horizontalScroll);
				int maxX = max(cursor.x, boxSelectStart->x - horizontalScroll);
				int minY = min(cursor.y, pianoRollBox.bottom - (boxSelectStart->y - verticalScroll));
				int maxY = max(cursor.y, pianoRollBox.bottom - (boxSelectStart->y - verticalScroll));
				for (int i = minY; i <= maxY; i++)
				{
					for (int j = minX; j <= maxX; j++)
					{
						if (j >= pianoRollBox.left && j <= pianoRollBox.right && i >= pianoRollBox.top && i <= pianoRollBox.bottom)
						{
							textdisp.set(j, i, textdisp.getChar(j, i), invertColour(textdisp.getAttributes(j, i)));
						}
					}
				}
			}
			else
			{
				//ritar pekare
				textdisp.set(cursor.x, cursor.y, textdisp.getChar(cursor.x, cursor.y), invertColour(textdisp.getAttributes(cursor.x, cursor.y)));
			}

			//ritar meny
			menu.draw(0, 0, textdisp);

			textdisp.draw();

			timer.wait();
		}
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unknown error", "Error", MB_OK | MB_ICONERROR);
	}
	return 0;
}