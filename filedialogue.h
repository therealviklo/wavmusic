#pragma once
#include <string>
#include <windows.h>
#include <stdexcept>

//glöm inte att du behöver länka till comdlg32 (flagga "-lcomdlg32" i g++)

//filetypeString ska ha beskrivningen av en filtyp, sedan nulltecken, sedan en matchningssträng för filtypen (eller flera som
//då separeras av semikolon), sedan nulltecken, sedan nästa par o.s.v. och till sist ska det vara två nulltecken för att 
//avsluta det hela. Exempel: L"JSON-filer\0*.json\0\0"

std::wstring openDialogueW(const wchar_t* filetypeString, const wchar_t* title, const wchar_t* autoExtension, HWND owner = NULL);
std::string openDialogueA(const char* filetypeString, const char* title, const char* autoExtension, HWND owner = NULL);