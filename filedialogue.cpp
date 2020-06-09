#pragma once
#include "filedialogue.h"

std::wstring openDialogueW(const wchar_t* filetypeString, const wchar_t* title, const wchar_t* autoExtension, HWND owner)
{
	wchar_t nameBuffer[2048] = {};
	OPENFILENAMEW ofn = { //herregud mer än 20 parametrar
		sizeof(OPENFILENAMEW),
		owner,
		NULL,
		filetypeString,
		NULL,
		0,
		1,
		nameBuffer,
		sizeof(nameBuffer),
		NULL,
		0,
		NULL,
		title,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		0,
		0,
		autoExtension,
		0,
		NULL,
		NULL,
		NULL,
		0,
		0
	};
	if (GetOpenFileNameW(&ofn) == 0) throw std::runtime_error("Error when getting filename");
	return nameBuffer;
}

std::string openDialogueA(const char* filetypeString, const char* title, const char* autoExtension, HWND owner)
{
	char nameBuffer[2048] = {};
	OPENFILENAMEA ofn = { //herregud mer än 20 parametrar
		sizeof(OPENFILENAMEW),
		owner,
		NULL,
		filetypeString,
		NULL,
		0,
		1,
		nameBuffer,
		sizeof(nameBuffer),
		NULL,
		0,
		NULL,
		title,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		0,
		0,
		autoExtension,
		0,
		NULL,
		NULL,
		NULL,
		0,
		0
	};
	if (GetOpenFileNameA(&ofn) == 0) throw std::runtime_error("Error when getting filename");
	return nameBuffer;
}